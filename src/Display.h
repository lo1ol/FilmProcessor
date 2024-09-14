#pragma once

#include <LiquidCrystal.h>

#include "Config.h"
#include "DisplayLine.h"

constexpr auto kAcceptSymbol = '\x01';
constexpr auto kBackSymbol = '\x02';
constexpr auto kLoad0Symbol = '\x03';
constexpr auto kLoad1Symbol = '\x04';
constexpr auto kLoad2Symbol = '\x05';
constexpr auto kLoad3Symbol = '\x06';
constexpr auto kLoad4Symbol = '\x07';
constexpr auto kLoad5Symbol = '\x08';

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
