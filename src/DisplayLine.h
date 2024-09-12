#pragma once

#include <Arduino.h>

#include <stdint.h>

#include "Config.h"

class LiquidCrystal;

class DisplayLine {
public:
    DisplayLine(int line, LiquidCrystal& lcd) : m_line(line), m_lcd(lcd) {}

    void tick();
    void reset();
    void resetBlink(bool state = false, uint16_t blinkSpeed = 500);

    void printBlink(char src);
    void printBlink(const char* src, bool right = false);

    void printHeader(const char* src);

    DisplayLine& operator<<(const char* src);
    DisplayLine& operator<<(char ch);
    DisplayLine& operator<<(int value);

    DisplayLine& operator>>(const char* src);
    DisplayLine& operator>>(int value);

private:
    static void concat(char* dst, const char* src);
    static void concat(char* dst, char src);
    static void concat(char* dst, int value);

    int m_line;
    LiquidCrystal& m_lcd;

    uint8_t m_blinkPos = 0;
    uint8_t m_blinkLength = 0;
    uint32_t m_blinkTimer = 0;
    bool m_blinkState = 0;
    uint16_t m_blinkSpeed;

    char m_fwInfo[DISPLAY_COLS + 1] = "";
    char m_bwInfo[DISPLAY_COLS + 1] = "";
};
