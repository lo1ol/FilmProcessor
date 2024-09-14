#pragma once

#include "../BaseMenu.h"
#include "../StepExecutor.h"

namespace Menu {

class ProcessExecutor : public BaseMenu {
public:
    ProcessExecutor();
    void tick() override;

private:
    void updateStep();

    StepExecutor m_stepExecutor;
    uint32_t m_totalTime = 0;
    uint32_t m_startTime = 0;
    uint8_t m_currentStep = 0;
};

} // namespace Menu
