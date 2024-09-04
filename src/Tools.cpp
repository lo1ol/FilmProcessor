#include "Tools.h"

#include <LiquidCrystal.h>

App gApp;
Display gDisplay(LiquidCrystal(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7));
EncButton gEncoder(ENCODER_DT, ENCODER_CLK);
ButtonT<MODE_SWITCH_BTN> gModeSwitchBtn;

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
