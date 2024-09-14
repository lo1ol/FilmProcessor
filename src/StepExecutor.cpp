#include "StepExecutor.h"

#include <Arduino.h>

#include "Config.h"
#include "MyAssert.h"

namespace {
void startLoadChem(uint8_t valvePin) {
    digitalWrite(valvePin, HIGH);
}

void stopLoadChem(uint8_t valvePin) {
    digitalWrite(valvePin, LOW);
}

void startUnLoadChem(uint8_t valvePin) {
    digitalWrite(valvePin, HIGH);
}

void stopUnLoadChem(uint8_t valvePin) {
    digitalWrite(valvePin, LOW);
}
} // namespace

StepExecutor::StepExecutor(const ProgDesc::Step& step) {
    m_stepTime = step.time * 1000L;
}

void StepExecutor::tick() {
    if (!m_startTime) {
        m_startTime = millis();
        return;
    }

    m_passedTime = millis() - m_startTime;
}

ChemStepExecutor::ChemStepExecutor(const ProgDesc::Step& step) : StepExecutor(step) {
    switch (step.action) {
    case ProgDesc::Action::Dev:
        m_targetValve = DEV_VALVE;
        m_isWaste = false;
        break;
    case ProgDesc::Action::Bleach:
        m_targetValve = BLEACH_VALVE;
        m_isWaste = false;
        break;
    case ProgDesc::Action::Fix:
        m_targetValve = FIX_VALVE;
        m_isWaste = false;
        break;
    case ProgDesc::Action::ExtraBath:
    case ProgDesc::Action::Dev2:
        m_targetValve = EXTRA_VALVE;
        m_isWaste = false;
        break;
    case ProgDesc::Action::Wash:
        m_targetValve = WATER_VALVE;
        m_isWaste = true;
        break;
    case ProgDesc::Action::Wait:
    case ProgDesc::Action::Finish:
    case ProgDesc::Action::last_:
        MyAssert(false);
        break;
    }

    m_stepTime = step.time * 1000L;
}

void ChemStepExecutor::tick() {
    StepExecutor::tick();
    switch (m_phase) {
    case Phase::NotStarted:
        startLoadChem(m_targetValve);
        m_phase = Phase::LoadChem;
        break;
    case Phase::LoadChem:
        if (m_passedTime < CHEM_LOAD_TIME)
            break;
        stopLoadChem(m_targetValve);
        m_phase = Phase::Execute;
        break;
    case Phase::Execute:
        if (m_passedTime + CHEM_LOAD_TIME < m_stepTime)
            break;

        if (m_isWaste)
            startUnLoadChem(WASTE_VALVE);
        else
            startUnLoadChem(m_targetValve);
        m_phase = Phase::UnloadChem;
        break;
    case Phase::UnloadChem:
    case Phase::Abort:
        if (m_passedTime < m_stepTime)
            break;

        if (m_isWaste)
            stopUnLoadChem(WASTE_VALVE);
        else
            stopUnLoadChem(m_targetValve);
        m_phase = Phase::Finished;
        break;
    case Phase::Finished:
        break;
    }
}

void ChemStepExecutor::abort() {
    switch (m_phase) {
    case Phase::NotStarted:
        m_stepTime = 0;
        break;
    case Phase::LoadChem:
        m_stepTime = m_passedTime + 1000;
        break;
    case Phase::Execute:
        m_stepTime = CHEM_LOAD_TIME;
        break;
    case Phase::UnloadChem:
        m_stepTime = m_stepTime - m_passedTime;
        break;
    case Phase::Finished:
    case Phase::Abort:
        return;
    }

    m_passedTime = 0;
    m_startTime = millis();
    startUnLoadChem(m_targetValve);
    m_phase = Phase::Abort;
}

WashStepExecutor::WashStepExecutor(const ProgDesc::Step& step)
    : StepExecutor(step), m_phaseWasher(getPhaseWasher(step.time * 1000)) {}

void WashStepExecutor::tick() {
    StepExecutor::tick();

    m_phaseWasher.tick();
    if (m_phaseWasher.finished()) {
        if (m_aborted || m_passedTime >= m_stepTime) {
            m_finished = true;
            return;
        }

        m_phaseWasher = getPhaseWasher(m_stepTime - m_passedTime);
    }
}

void WashStepExecutor::abort() {
    if (m_aborted)
        return;
    m_aborted = true;
    m_phaseWasher.abort();

    m_startTime = millis();
    m_stepTime = m_phaseWasher.stepTime();
    m_passedTime = m_phaseWasher.passedTime();
}

ChemStepExecutor WashStepExecutor::getPhaseWasher(uint32_t time) {
    ProgDesc::Step step{ .action = ProgDesc::Action::Wash };
    if (time >= 4 * CHEM_LOAD_TIME + PURE_WASH_TIME * 2)
        step.time = (2 * CHEM_LOAD_TIME + PURE_WASH_TIME) / 1000;
    else
        step.time = lround(time / 1000.);

    return ChemStepExecutor(step);
}
