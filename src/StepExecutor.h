#pragma once

#include "ProgDesc.h"

class StepExecutor {
public:
    StepExecutor(const ProgDesc::Step& step);
    virtual ~StepExecutor() = default;

    virtual void tick();
    virtual bool finished() const = 0;
    virtual void abort() = 0;

    uint32_t passedTime() const;
    uint32_t stepTime() const { return m_stepTime; }
    bool preparing() { return !m_started; }

protected:
    void start();

    uint32_t m_stepTime;
    uint32_t m_startTime = 0;
    uint32_t m_passedTime = 0;
    bool m_started = false;
};

class ChemStepExecutor : public StepExecutor {
public:
    ChemStepExecutor(const ProgDesc::Step& step, bool needCleanTube);

    void tick() override;
    bool finished() const override { return m_phase == Phase::Finished; }
    void abort() override;

private:
    enum class Phase : uint8_t {
        StartCleanTube,
        CleanTube,
        NotStarted,
        LoadChem,
        Execute,
        UnloadChem,
        Finished,
        Abort,
    };

    Phase m_phase;
    uint8_t m_targetValve;
    bool m_isWaste;
};

class WashStepExecutor : public StepExecutor {
public:
    WashStepExecutor(const ProgDesc::Step& step, bool needCleanTube);

    void tick() override;
    bool finished() const override { return m_finished; }
    void abort() override;

private:
    ChemStepExecutor getPhaseWasher(uint16_t time, bool needCleanTube = false);

    ChemStepExecutor m_phaseWasher;
    bool m_finished = false;
    bool m_aborted = false;
    uint16_t m_restTime;
};
