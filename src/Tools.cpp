#include "Tools.h"

#include "ProcessSettings.h"

#include <LiquidCrystal.h>

#include <string.h>

App gApp;
Display gDisplay(LiquidCrystal(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7));
EncButton gEncoder(ENCODER_DT, ENCODER_CLK);
ButtonT<MODE_SWITCH_BTN> gModeSwitchBtn;
ButtonT<BACK_BTN> gBackBtn;
Memory gMemory;
Rotator gRotator;
Pump gPump;
MelodyPlayer gMelodyPlayer;

void isr() {
    gEncoder.tickISR();
}

void setupEncoder() {
    attachInterrupt(0, isr, CHANGE);
    attachInterrupt(1, isr, CHANGE);
    gEncoder.setEncISR(true);
}

int8_t getEncoderDir() {
    if (!gEncoder.turn())
        return 0;
    return gEncoder.dir();
}

int8_t getEncoderShift() {
    int8_t shift = getEncoderDir();
    if (gEncoder.fast())
        shift *= 5;

    return shift;
}

bool getTime(uint16_t& time) {
    int8_t shift = getEncoderShift();
    auto oldTime = time;
    int16_t sTime = time;

    int16_t factor;
    if ((sTime + shift) < 60)
        factor = 1;
    else if ((sTime + shift) < 60 * 10)
        factor = 5;
    else if ((sTime + shift) < 60 * 30)
        factor = 10;
    else
        factor = 30;

    sTime += shift * factor;
    sTime = (sTime / factor) * factor;

    constexpr int32_t kMinTimeMs =
        CHEM_LOAD_ML_SPEED * static_cast<uint32_t>(ProcessSettings::Volume::ml700) * 2 + POST_CLEAN_TUBES_TIME;
    constexpr int16_t kMinTimeS = kMinTimeMs / 1000 + (kMinTimeMs % 1000 ? 1 : 0);
    if (sTime < kMinTimeS)
        sTime = kMinTimeS;

    if (sTime > 60 * 60)
        sTime = 60 * 60;

    time = sTime;

    return time != oldTime;
}

void formatTime(uint16_t time, char* dst) {
    uint8_t secs = time % 60;
    uint8_t mins = time / 60;

    itoa(mins, dst, 10);
    strcat(dst, ":");
    if (secs < 10)
        strcat(dst, "0");

    itoa(secs, dst + strlen(dst), 10);
}

uint16_t unformatTime(const char* str_) {
    char str[6];
    strcpy(str, str_);
    auto delim = strchr(str, ':');
    *delim = 0;
    auto min = atoi(str);
    auto sec = atoi(delim + 1);

    return min * 60 + sec;
}
