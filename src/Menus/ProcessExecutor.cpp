#include "ProcessExecutor.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessExecutor::ProcessExecutor() {
    const auto& prog = gMemory.getProg();
    for (uint8_t id = 0; ProgDesc::Action::Finish != prog.steps[id].action; ++id) {
        m_totalTime += prog.steps[id].time;
    }

    m_totalTime *= 1000;
}

void ProcessExecutor::tick() {
    const auto& prog = gMemory.getProg();

    if (!m_startTime) {
        m_startTime = millis();
        updateStep();
    }

    m_stepExecutor.tick();

    gDisplay[0].printHeader(prog.name);
    gDisplay[1] << "Step: " << prog.getStepName(m_currentStep);
    char formatedTime[7];
    formatTime(m_stepExecutor.passedTime(), formatedTime);
    gDisplay[2] << "Step t: " >> formatedTime;
    formatTime((millis() - m_startTime) / 1000, formatedTime);
    gDisplay[3] << "Total t: " >> formatedTime;

    if (!m_stepExecutor.finished())
        return;

    ++m_currentStep;
    updateStep();
}

void ProcessExecutor::updateStep() {
    const auto& step = gMemory.getProg().steps[m_currentStep];
    switch (step.action) {
    case ProgDesc::Action::Finish:
        gApp.setMenu(new ProcessMenu());
        return;
    case ProgDesc::Action::Wait:
        // TODO
        break;
    case ProgDesc::Action::Dev:
    case ProgDesc::Action::Bleach:
    case ProgDesc::Action::Fix:
    case ProgDesc::Action::Dev2:
    case ProgDesc::Action::ExtraBath:
    case ProgDesc::Action::Wash:
        m_stepExecutor = StepExecutor(step);
        break;
    case ProgDesc::Action::last_:
        break;
    }
}

} // namespace Menu
