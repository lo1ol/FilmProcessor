#pragma once

#include "../BaseMenu.h"
#include "../ConfirmAsker.h"
#include "../StepExecutor.h"

namespace Menu {

class ProcessExecutor : public BaseMenu {
public:
    ProcessExecutor();
    ~ProcessExecutor();
    void tick() override;

private:
    void updateStep();
    void nextStep();
    void printProgressInfo() const;

    enum class View : uint8_t { PassedTime, RestTime, last_ };

    enum class Phase {
        Normal,
        OnBack,
        OnFinish,
        OnWait,
        OnAbort,
    };

    StepExecutor* m_stepExecutor = nullptr;
    uint32_t m_totalTime = 0;
    uint32_t m_prevStepsTime = 0;
    uint8_t m_currentStep = 0;
    View m_view = View::PassedTime;
    Phase m_phase;
    ConfirmAsker m_confirmAsker;
    bool m_needCleanTube = true;
};

} // namespace Menu
