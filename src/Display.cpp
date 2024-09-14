#include "Display.h"

namespace {

uint8_t kAcceptChar[] = { 0b00001, 0b00001, 0b00001, 0b00010, 0b10010, 0b01010, 0b00100, 0b00000 };

uint8_t kBackChar[] = { 0b00000, 0b00100, 0b01000, 0b11111, 0b01000, 0b00100, 0b00000, 0b00000 };

uint8_t kLoad0Char[] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
uint8_t kLoad1Char[] = { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000 };
uint8_t kLoad2Char[] = { 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000 };
uint8_t kLoad3Char[] = { 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100 };
uint8_t kLoad4Char[] = { 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110 };
uint8_t kLoad5Char[] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };

} // namespace

Display::Display(LiquidCrystal&& lcd)
    : m_lcd(lcd)
    , m_lines({ DisplayLine(0, m_lcd), DisplayLine(1, m_lcd), DisplayLine(2, m_lcd), DisplayLine(3, m_lcd) }) {
    m_lcd.begin(DISPLAY_COLS, DISPLAY_ROWS);
    m_lcd.createChar(kAcceptSymbol, kAcceptChar);
    m_lcd.createChar(kBackSymbol, kBackChar);
    m_lcd.createChar(kLoad0Symbol, kLoad0Char);
    m_lcd.createChar(kLoad1Symbol, kLoad1Char);
    m_lcd.createChar(kLoad2Symbol, kLoad2Char);
    m_lcd.createChar(kLoad3Symbol, kLoad3Char);
    m_lcd.createChar(kLoad4Symbol, kLoad4Char);
    m_lcd.createChar(kLoad5Symbol, kLoad5Char);
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
