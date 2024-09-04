#include "Display.h"

namespace {

uint8_t kAcceptChar[] = { 0b00001, 0b00001, 0b00001, 0b00010, 0b10010, 0b01010, 0b00100, 0b00000 };

uint8_t kBackChar[] = { 0b00000, 0b00100, 0b01000, 0b11111, 0b01000, 0b00100, 0b00000, 0b00000

};
} // namespace

Display::Display(LiquidCrystal&& lcd)
    : m_lcd(lcd)
    , m_lines({ DisplayLine(0, m_lcd), DisplayLine(1, m_lcd), DisplayLine(2, m_lcd), DisplayLine(3, m_lcd) }) {
    m_lcd.begin(DISPLAY_COLS, DISPLAY_ROWS);
    m_lcd.createChar(kAcceptSymbol, kAcceptChar);
    m_lcd.createChar(kBackSymbol, kBackChar);
}

void Display::tick() {
    for (auto&& line : m_lines)
        line.tick();
}

void Display::reset() {
    for (auto&& line : m_lines)
        line.reset();
}

void Display::resetBlink(bool state, uint16_t blinkState) {
    for (auto&& line : m_lines)
        line.resetBlink(state, blinkState);
}
