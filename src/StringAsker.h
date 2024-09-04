#pragma once

#include <Arduino.h>

class StringAsker {
public:
    StringAsker(const char* greeting = "", const String& base = "");
    bool finish() const { return m_finish; }
    String result() const { return m_result; }

    void tick();

public:
    String m_result;
    const char* m_greeting;
    bool m_finish;
    uint8_t m_pos;
};
