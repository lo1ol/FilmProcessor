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

    printProgressString(prog.steps[m_currentStep].time, m_stepExecutor.passedTime(), 2);
    formatTime(m_stepExecutor.passedTime(), formatedTime);
    gDisplay[2] >> formatedTime;

    auto passedTime = millis() - m_startTime;
    printProgressString(m_totalTime, passedTime, 3);
    formatTime(passedTime / 1000, formatedTime);
    gDisplay[3] >> formatedTime;

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
