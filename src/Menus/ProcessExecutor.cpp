#include "ProcessExecutor.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessExecutor::ProcessExecutor() {
    const auto& prog = gMemory.getProg();
    for (uint8_t id = 0; ProgDesc::Action::Finish != prog.steps[id].action; ++id)
        if (prog.stepSupportTime(id))
            m_totalTime += prog.steps[id].time;

    m_totalTime *= 1000L;
    updateStep();
}
ProcessExecutor::~ProcessExecutor() {
    if (m_stepExecutor)
        delete m_stepExecutor;
    gRotator.stop();
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
            m_stepExecutor->abort();
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

    m_stepExecutor->tick();

    if (gBackBtn.click() && m_phase == Phase::Normal) {
        m_phase = Phase::OnBack;
        m_confirmAsker = ConfirmAsker("Abort process");
    }

    if (!m_stepExecutor->finished())
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
    if (m_stepExecutor->preparing())
        gDisplay[1] << "Step: Clean tube";
    else if (m_phase == Phase::OnAbort)
        gDisplay[1] << "Step: Aborting";
    else
        gDisplay[1] << "Step: " << prog.getStepName(m_currentStep);

    gDisplay[2].printTimeProgress(m_stepExecutor->stepTime(), m_stepExecutor->passedTime(), m_view == View::RestTime);

    if (m_phase == Phase::OnAbort)
        return;

    auto passedTime = m_prevStepsTime + m_stepExecutor->passedTime();

    gDisplay[3].printTimeProgress(m_totalTime, passedTime, m_view == View::RestTime);
}

void ProcessExecutor::nextStep() {
    const auto& prog = gMemory.getProg();

    if (prog.stepSupportTime(m_currentStep))
        m_prevStepsTime += prog.steps[m_currentStep].time * 1000L;
    ++m_currentStep;
    updateStep();
}

void ProcessExecutor::updateStep() {
    const auto& step = gMemory.getProg().steps[m_currentStep];

    if (m_stepExecutor) {
        delete m_stepExecutor;
        m_stepExecutor = nullptr;
    }

    switch (step.action) {
    case ProgDesc::Action::Finish:
        gRotator.stop();
        m_confirmAsker = ConfirmAsker("Finish!", "", ConfirmAsker::Type::ClickConfirm);
        m_phase = Phase::OnFinish;
        break;
    case ProgDesc::Action::Wait:
        gRotator.stop();
        m_confirmAsker = ConfirmAsker("Wait for actions", "", ConfirmAsker::Type::HoldConfirm);
        m_phase = Phase::OnWait;
        m_needCleanTube = true;
        break;
    case ProgDesc::Action::Dev:
    case ProgDesc::Action::Bleach:
    case ProgDesc::Action::Fix:
    case ProgDesc::Action::Dev2:
    case ProgDesc::Action::ExtraBath:
        gRotator.start();
        m_stepExecutor = new ChemStepExecutor(step, m_needCleanTube);
        m_phase = Phase::Normal;
        m_needCleanTube = false;
        break;
    case ProgDesc::Action::Wash:
        gRotator.start();
        m_stepExecutor = new WashStepExecutor(step, m_needCleanTube);
        m_phase = Phase::Normal;
        m_needCleanTube = false;
        break;
    case ProgDesc::Action::last_:
        MyAssert(false);
    }
}

} // namespace Menu
