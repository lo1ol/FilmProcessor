#pragma once

#include "Config.h"

#include <stdint.h>

class MachineCleaner {
public:
    MachineCleaner();

    void tick();

    static constexpr uint32_t totalTime() { return 5 * CLEAN_BATH_TIME + 4 * FREE_BATH_TIME + POST_CLEAN_TUBES_TIME; }
    uint32_t passedTime() const;
    bool finish() const { return m_finish; }
    const char* stepName() const { return m_stepName; }

private:
    enum class Phase : uint8_t {
        NotStarted,
        LoadTank,
        LoadExtra,
        UnLoadExtra,
        LoadDev,
        UnLoadDev,
        LoadBleach,
        UnLoadBleach,
        LoadFix,
        UnLoadFix,
        CleanTube,
        Finish,
        last_
    };

    Phase m_phase = Phase::NotStarted;
    uint32_t m_startTime = 0;
    uint32_t m_phaseStopTime = 0;
    bool m_finish = false;
    const char* m_stepName = "";
};
