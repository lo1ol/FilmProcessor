#include "MachineCleaner.h"

#include <Arduino.h>

#include "Tools.h"

MachineCleaner::MachineCleaner() {}

uint32_t MachineCleaner::passedTime() const {
    if (m_startTime > totalTime())
        return totalTime();
    if (m_startTime)
        return millis() - m_startTime;
    return 0;
}

void MachineCleaner::tick() {
    if (m_finish || millis() < m_phaseStopTime)
        return;

    uint32_t phaseTime;
    m_phase = ADD_TO_ENUM(Phase, m_phase, 1);

    auto loadStage = [&](uint8_t prevGear, uint8_t curGear, const char* desc) {
        digitalWrite(WASTE_VALVE, LOW);
        if (prevGear)
            digitalWrite(prevGear, LOW);
        digitalWrite(curGear, HIGH);
        digitalWrite(WATER_VALVE, HIGH);
        phaseTime = CLEAN_BATH_TIME;
        m_stepName = desc;
    };

    auto unloadStage = [&](uint8_t curGear) {
        digitalWrite(WATER_VALVE, LOW);
        digitalWrite(WASTE_VALVE, HIGH);
        digitalWrite(curGear, LOW);
        phaseTime = FREE_BATH_TIME;
    };

    switch (m_phase) {
    case Phase::NotStarted:
        MyAssert(false);
    case Phase::LoadTank:
        m_startTime = millis();
        digitalWrite(WATER_VALVE, HIGH);
        gPump.loadInTank();
        phaseTime = CLEAN_BATH_TIME;
        m_stepName = "Clean tank";
        break;
    case Phase::LoadExtra:
        gPump.stop();
        loadStage(0, EXTRA_VALVE, "Clean extra bath");
        break;
    case Phase::UnLoadExtra:
        unloadStage(EXTRA_VALVE);
        break;
    case Phase::LoadDev:
        loadStage(EXTRA_VALVE, DEV_VALVE, "Clean dev bath");
        break;
    case Phase::UnLoadDev:
        unloadStage(DEV_VALVE);
        break;
    case Phase::LoadBleach:
        loadStage(DEV_VALVE, BLEACH_VALVE, "Clean bleach bat");
        break;
    case Phase::UnLoadBleach:
        unloadStage(BLEACH_VALVE);
        break;
    case Phase::LoadFix:
        loadStage(BLEACH_VALVE, FIX_VALVE, "Clean fix bath");
        break;
    case Phase::UnLoadFix:
        unloadStage(FIX_VALVE);
        break;
    case Phase::CleanTube:
        gPump.loadOutTank();
        phaseTime = POST_CLEAN_TUBES_TIME;
        m_stepName = "Clean tubes";
        break;
    case Phase::Finish:
        gPump.stop();
        digitalWrite(WASTE_VALVE, LOW);
        m_finish = true;
        phaseTime = 0;
        m_stepName = "Finish";
        break;
    case Phase::last_:
        MyAssert(false);
    }
    m_phaseStopTime = millis() + phaseTime;
}
