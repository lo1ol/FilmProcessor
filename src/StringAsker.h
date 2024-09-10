#pragma once

#include <Arduino.h>

class StringAsker {
public:
    StringAsker(const char* greeting = "", const char* base = "");
    bool finish() const { return m_finish; }
    const char* result() const { return m_result; }

    void tick();

public:
    const char* m_greeting;
    bool m_finish = false;
    uint8_t m_pos;
    char m_result[11] = {};
};
