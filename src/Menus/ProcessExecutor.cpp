#include "ProcessExecutor.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

namespace {

void printProgressString(uint32_t max, uint32_t cur, uint8_t line) {
    char progressString[11];
    auto progress = (cur * 5 * 10) / max;
    auto filledCells = progress / 5;
    char unfilledCell = kLoad0Symbol + progress % 5;
    for (uint8_t i = 0; i != 10; ++i) {
        if (i < filledCells)
            progressString[i] = kLoad5Symbol;
        else if (i == filledCells)
            progressString[i] = unfilledCell;
        else
            progressString[i] = 0;
    }
    progressString[10] = 0;

    gDisplay[line] << progressString;
}

} // namespace

ProcessExecutor::ProcessExecutor() {
    const auto& prog = gMemory.getProg();
    for (uint8_t id = 0; ProgDesc::Action::Finish != prog.steps[id].action; ++id)
        if (prog.stepSupportTime(id))
            m_totalTime += prog.steps[id].time;

    m_totalTime *= 1000;
    updateStep();
}

void ProcessExecutor::tick() {
    switch (m_phase) {
    case Phase::Normal:
    case Phase::OnAbort:
        if (gModeSwitchBtn.click())
            m_view = ADD_TO_ENUM(View, m_view, 1);

        printProgressInfo();
        break;
    case Phase::OnBack:
        m_confirmAsker.tick();
        if (!m_confirmAsker.finish())
            break;

        if (m_confirmAsker.result()) {
            m_stepExecutor.abort();
            m_phase = Phase::OnAbort;
        } else {
            m_phase = Phase::Normal;
        }
        break;
    case Phase::OnWait:
        m_confirmAsker.tick();
        if (!m_confirmAsker.finish())
            return;
        nextStep();
        break;
    case Phase::OnFinish:
        m_confirmAsker.tick();
        if (!m_confirmAsker.finish())
            return;
        gApp.setMenu(new ProcessMenu());
        return;
    }

    m_stepExecutor.tick();

    if (gBackBtn.click() && m_phase == Phase::Normal) {
        m_phase = Phase::OnBack;
        m_confirmAsker = ConfirmAsker("Wonna stop?");
    }

    if (!m_stepExecutor.finished())
        return;

    if (m_phase == Phase::OnAbort) {
        gApp.setMenu(new ProcessMenu());
        return;
    }

    nextStep();
}

void ProcessExecutor::printProgressInfo() const {
    const auto& prog = gMemory.getProg();

    gDisplay[0].printHeader(prog.name);
    if (m_phase == Phase::OnAbort)
        gDisplay[1] << "Step: Aborting";
    else
        gDisplay[1] << "Step: " << prog.getStepName(m_currentStep);

    printProgressString(m_stepExecutor.stepTime(), m_stepExecutor.passedTime(), 2);

    char formatedTime[7];
    switch (m_view) {
    case View::PassedTime:
        formatTime(m_stepExecutor.passedTime() / 1000, formatedTime);
        gDisplay[2] >> formatedTime;
        break;
    case View::RestTime:
        formatTime((m_stepExecutor.stepTime() - m_stepExecutor.passedTime()) / 1000, formatedTime);
        gDisplay[2] >> formatedTime;
        break;
    case View::last_:
        MyAssert(false);
    }

    if (m_phase == Phase::OnAbort)
        return;

    auto passedTime = m_prevStepsTime + m_stepExecutor.passedTime();
    printProgressString(m_totalTime, passedTime, 3);

    switch (m_view) {
    case View::PassedTime:
        formatTime(passedTime / 1000, formatedTime);
        gDisplay[3] >> formatedTime;
        break;
    case View::RestTime:
        formatTime((m_totalTime - passedTime) / 1000, formatedTime);
        gDisplay[3] >> formatedTime;
        break;
    case View::last_:
        MyAssert(false);
    }
}

void ProcessExecutor::nextStep() {
    const auto& prog = gMemory.getProg();

    if (prog.stepSupportTime(m_currentStep))
        m_prevStepsTime += prog.steps[m_currentStep].time * 1000;
    ++m_currentStep;
    updateStep();
}

void ProcessExecutor::updateStep() {
    const auto& step = gMemory.getProg().steps[m_currentStep];

    switch (step.action) {
    case ProgDesc::Action::Finish:
        m_confirmAsker = ConfirmAsker("Finish!", ConfirmAsker::Type::ClickConfirm);
        m_phase = Phase::OnFinish;
        break;
    case ProgDesc::Action::Wait:
        m_confirmAsker = ConfirmAsker("Wait for actions", ConfirmAsker::Type::HoldConfirm);
        m_phase = Phase::OnWait;
        break;
    case ProgDesc::Action::Dev:
    case ProgDesc::Action::Bleach:
    case ProgDesc::Action::Fix:
    case ProgDesc::Action::Dev2:
    case ProgDesc::Action::ExtraBath:
    case ProgDesc::Action::Wash:
        m_stepExecutor = StepExecutor(step);
        m_phase = Phase::Normal;
        break;
    case ProgDesc::Action::last_:
        break;
    }
}

} // namespace Menu
