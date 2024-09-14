#pragma once

#include "ProgDesc.h"

class StepExecutor {
public:
    StepExecutor(const ProgDesc::Step& step);
    virtual ~StepExecutor() = default;

    virtual void tick() = 0;
    virtual bool finished() const = 0;
    virtual void abort() = 0;

    uint32_t passedTime() const;
    uint32_t stepTime() const { return m_stepTime; }

protected:
    uint32_t m_stepTime;
    uint32_t m_startTime = 0;
};

class ChemStepExecutor : public StepExecutor {
public:
    ChemStepExecutor(const ProgDesc::Step& step);

    void tick() override;
    bool finished() const override { return m_phase == Phase::Finished; }
    void abort() override;

private:
    enum class Phase : uint8_t {
        NotStarted,
        LoadChem,
        Execute,
        UnloadChem,
        Finished,
        Abort,
    };

    Phase m_phase = Phase::NotStarted;
    uint8_t m_targetValve;
};
