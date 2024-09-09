#include "Tools.h"

#include <LiquidCrystal.h>

App gApp;
Display gDisplay(LiquidCrystal(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7));
EncButton gEncoder(ENCODER_DT, ENCODER_CLK);
ButtonT<MODE_SWITCH_BTN> gModeSwitchBtn;
ButtonT<BACK_BTN> gBackBtn;
Memory gMemory;

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

    if (sTime < 0)
        sTime = 0;

    if (sTime > 60 * 60)
        sTime = 60 * 60;

    time = sTime;

    return time != oldTime;
}

String formatTime(uint16_t time) {
    String res;

    res.reserve(6);

    uint8_t secs = time % 60;
    uint8_t mins = time / 60;

    res = String(mins);
    res += ':';
    if (secs < 10)
        res += '0';
    res += String(secs);

    return res;
}

uint16_t unformatTime(String str) {
    auto delim = str.indexOf(':');
    auto minStr = str.substring(0, delim);
    auto secStr = str.substring(delim + 1);

    auto min = minStr.toInt();
    auto sec = secStr.toInt();

    return min * 60 + sec;
}
