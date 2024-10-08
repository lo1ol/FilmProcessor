#include "DisplayLine.h"

#include <LiquidCrystal.h>

#include "Tools.h"

void DisplayLine::concat(char* dst, const char* src) {
    int srcLen = strlen(src);
    int shift = strlen(dst);
    int rest = DISPLAY_COLS - shift;

    srcLen = min(srcLen, rest);

    memcpy(dst + shift, src, srcLen);
    dst[shift + srcLen] = 0;
}

void DisplayLine::concat(char* dst, char ch) {
    int srcLen = 1;
    int shift = strlen(dst);
    int rest = DISPLAY_COLS - shift;

    srcLen = min(srcLen, rest);

    dst[shift] = ch;
    dst[shift + srcLen] = 0;
}

void DisplayLine::concat(char* dst, int value) {
    char str[DISPLAY_COLS + 1];
    itoa(value, str, 10);
    concat(dst, str);
}

void DisplayLine::reset() {
    m_fwInfo[0] = 0;
    m_bwInfo[0] = 0;
    m_blinkLength = 0;
    m_blinkSpeed = 500;
}

void DisplayLine::resetBlink(bool state, uint16_t blinkSpeed) {
    m_blinkTimer = millis();
    m_blinkState = state;
    m_blinkSpeed = blinkSpeed;
}

void DisplayLine::tick() {
    auto fwLen = strlen(m_fwInfo);
    memset(m_fwInfo + fwLen, ' ', DISPLAY_COLS - fwLen);

    auto bwLen = strlen(m_bwInfo);
    memcpy(m_fwInfo + DISPLAY_COLS - bwLen, m_bwInfo, bwLen);

    if (m_blinkLength) {
        if (millis() - m_blinkTimer > m_blinkSpeed) {
            m_blinkState = !m_blinkState;
            m_blinkTimer = millis();
        }

        if (m_blinkState)
            memset(m_fwInfo + m_blinkPos, ' ', m_blinkLength);
    }

    m_lcd.setCursor(0, m_line);

    m_lcd.print(m_fwInfo);

    reset();
}

DisplayLine& DisplayLine::operator<<(const char* src) {
    concat(m_fwInfo, src);
    return *this;
}

DisplayLine& DisplayLine::operator<<(char src) {
    concat(m_fwInfo, src);
    return *this;
}

DisplayLine& DisplayLine::operator<<(int value) {
    concat(m_fwInfo, value);
    return *this;
}

DisplayLine& DisplayLine::operator>>(const char* src) {
    concat(m_bwInfo, src);
    return *this;
}

DisplayLine& DisplayLine::operator>>(int value) {
    concat(m_bwInfo, value);
    return *this;
}

void DisplayLine::printBlink(char src) {
    m_blinkPos = strlen(m_fwInfo);
    m_blinkLength = 1;
    *this << src;
}

void DisplayLine::printBlink(const char* src, bool right) {
    m_blinkLength = strlen(src);

    if (right) {
        m_blinkPos = DISPLAY_COLS - strlen(m_bwInfo) - strlen(src);
        *this >> src;
    } else {
        m_blinkPos = strlen(m_fwInfo);
        *this << src;
    }
}

void DisplayLine::printHeader(const char* src) {
    uint8_t strLen = strlen(src) + 2;
    uint8_t leftPadding = ((DISPLAY_COLS - strLen) / 2) + ((DISPLAY_COLS - strLen) % 2);
    uint8_t rightPadding = DISPLAY_COLS - strLen - leftPadding;

    for (uint8_t i = 0; i != leftPadding; ++i)
        *this << '*';
    *this << ' ';
    *this << src;
    *this << ' ';
    for (uint8_t i = 0; i != rightPadding; ++i)
        *this << '*';
}

void DisplayLine::printTimeProgress(uint32_t maxTime, uint32_t curTime, bool rest) {
    uint32_t printedTime;

    if (rest) {
        if (curTime >= maxTime)
            printedTime = 0;
        else
            printedTime = maxTime - curTime;
    } else {
        printedTime = curTime;
    }

    char formatedTime[7];
    formatTime(printedTime / 1000, formatedTime);
    *this >> formatedTime;

    char progressString[11];
    auto progress = (curTime * 5L * 10L) / maxTime;
    auto filledCells = progress / 5;
    char unfilledCell = kLoad0Symbol + progress % 5;
    for (uint8_t i = 0; i != 10; ++i) {
        if (i < filledCells)
            progressString[i] = kLoad5Symbol;
        else if (i == filledCells)
            progressString[i] = unfilledCell;
        else
            progressString[i] = 0;
    }
    progressString[10] = 0;

    *this << progressString;
}
