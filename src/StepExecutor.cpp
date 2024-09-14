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
    switch (step.action) {
    case ProgDesc::Action::Dev:
        m_targetValve = DEV_VALVE;
        m_waste = false;
        break;
    case ProgDesc::Action::Bleach:
        m_targetValve = BLEACH_VALVE;
        m_waste = false;
        break;
    case ProgDesc::Action::Fix:
        m_targetValve = FIX_VALVE;
        m_waste = false;
        break;
    case ProgDesc::Action::ExtraBath:
    case ProgDesc::Action::Dev2:
        m_targetValve = EXTRA_VALVE;
        m_waste = false;
        break;
    case ProgDesc::Action::Wash:
        m_targetValve = WATER_VALVE;
        m_waste = true;
        break;
    case ProgDesc::Action::Wait:
    case ProgDesc::Action::Finish:
    case ProgDesc::Action::last_:
        MyAssert(false);
        break;
    }

    m_stepTime = step.time * 1000;
}

void StepExecutor::tick() {
    uint32_t currentTime = millis() - m_startTime;
    switch (m_phase) {
    case Phase::NotStarted:
        Serial.println(m_targetValve);
        startLoadChem(m_targetValve);
        m_startTime = millis();
        m_phase = Phase::LoadChem;
        break;
    case Phase::LoadChem:
        if (currentTime < CHEM_LOAD_TIME)
            break;
        stopLoadChem(m_targetValve);
        m_phase = Phase::Execute;
        break;
    case Phase::Execute:
        if ((currentTime + CHEM_LOAD_TIME) < m_stepTime)
            break;
        if (m_waste)
            startUnLoadChem(WASTE_VALVE);
        else
            startUnLoadChem(m_targetValve);
        m_phase = Phase::UnloadChem;
        break;
    case Phase::UnloadChem:
        if (currentTime < m_stepTime)
            break;

        if (m_waste)
            stopUnLoadChem(WASTE_VALVE);
        else
            stopUnLoadChem(m_targetValve);
        m_phase = Phase::Finished;
        break;
    case Phase::Finished:
        break;
    }
}

uint32_t StepExecutor::passedTime() const {
    if (!m_startTime)
        return 0;

    return millis() - m_startTime;
}
