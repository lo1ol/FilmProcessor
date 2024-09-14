#pragma once

#include "ProgDesc.h"

class StepExecutor {
public:
    StepExecutor(const ProgDesc::Step& step);
    virtual ~StepExecutor() = default;

    virtual void tick();
    virtual bool finished() const = 0;
    virtual void abort() = 0;

    uint32_t passedTime() const { return m_passedTime; }
    uint32_t stepTime() const { return m_stepTime; }

protected:
    uint32_t m_stepTime;
    uint32_t m_startTime = 0;
    uint32_t m_passedTime = 0;
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
    bool m_isWaste;
};

class WashStepExecutor : public StepExecutor {
public:
    WashStepExecutor(const ProgDesc::Step& step);

    void tick() override;
    bool finished() const override { return m_finished; }
    void abort() override;

private:
    ChemStepExecutor getPhaseWasher(uint32_t time);

    ChemStepExecutor m_phaseWasher;
    bool m_finished = false;
    bool m_aborted = false;
};
