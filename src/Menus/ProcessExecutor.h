#pragma once

#include "../BaseMenu.h"
#include "../ConfirmAsker.h"
#include "../StepExecutor.h"

namespace Menu {

class ProcessExecutor : public BaseMenu {
public:
    ProcessExecutor();
    void tick() override;

private:
    void updateStep();
    void printProgressInfo() const;

    enum class View : uint8_t { PassedTime, RestTime, last_ };

    enum class Phase {
        Normal,
        OnBack,
        OnFinish,
        OnWait,
    };

    StepExecutor m_stepExecutor;
    uint32_t m_totalTime = 0;
    uint32_t m_startTime = 0;
    uint8_t m_currentStep = 0;
    View m_view = View::PassedTime;
    Phase m_phase;
    ConfirmAsker m_confirmAsker;
};

} // namespace Menu
