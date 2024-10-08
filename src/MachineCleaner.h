#pragma once

#include "Config.h"

#include <stdint.h>

class MachineCleaner {
public:
    MachineCleaner();

    void tick();

    static constexpr uint8_t kIterCount = 6;
    static constexpr uint32_t kPhaseTime = CLEAN_PUMP_TIME + 4 * (FREE_BATH_TIME + CLEAN_BATH_TIME);
    static constexpr uint32_t totalTime() { return kIterCount * kPhaseTime + 4 * POST_CLEAN_TUBES_TIME; }
    uint32_t passedTime() const;
    bool finish() const { return m_finish; }
    const char* stepName() const { return m_stepName; }
    const char* confirmName() const;
    bool needConfirm() const;
    void nextPhase();

private:
    enum class Phase : uint8_t {
        ConfirmEmpty,
        ConfirmChangePipe,
        CleanPump,
        LoadExtra,
        UnLoadExtra,
        LoadDev,
        UnLoadDev,
        LoadBleach,
        UnLoadBleach,
        LoadFix,
        UnLoadFix,
        ConfirmCleanOutPipe,
        CleanTube,
        Finish,
        last_
    };

    Phase m_phase = Phase::ConfirmEmpty;
    uint32_t m_startTime = 0;
    uint32_t m_phaseStopTime = 0;
    bool m_finish = false;
    int8_t m_iteration = 1;
    const char* m_stepName = "";
};
