#pragma once

#include "ProgDesc.h"

class StepExecutor {
public:
    StepExecutor() = default;
    StepExecutor(const ProgDesc::Step& step);

    void tick();
    bool finished() const { return m_phase == Phase::Finished; }
    uint32_t passedTime() const;
    uint32_t stepTime() const { return m_stepTime; }
    void abort();

private:
    enum class Phase : uint8_t {
        NotStarted,
        LoadChem,
        Execute,
        UnloadChem,
        Finished,
        Abort,
    };

    uint32_t m_stepTime;
    uint32_t m_startTime = 0;
    Phase m_phase = Phase::NotStarted;
    uint8_t m_targetValve;
    bool m_waste;
};
