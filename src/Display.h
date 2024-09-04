#pragma once

#include <LiquidCrystal.h>

#include "Config.h"
#include "DisplayLine.h"

constexpr auto kAcceptSymbol = 0x01;
constexpr auto kBackSymbol = 0x02;

class Display {
public:
    Display(LiquidCrystal&& lcd);
    DisplayLine& operator[](uint8_t n) { return m_lines[n]; }
    DisplayLine* getLines() { return m_lines; }

    void tick();
    void reset();
    void resetBlink(bool state = false, uint16_t blinkSpeed = 500);

private:
    LiquidCrystal m_lcd;
    DisplayLine m_lines[DISPLAY_ROWS];
};
