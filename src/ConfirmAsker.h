#pragma once

#include <Arduino.h>

class ConfirmAsker {
public:
    ConfirmAsker(const char* greeting = "Are you sure?");
    bool finish() const { return m_finish; }
    bool result() const { return m_result; }

    void tick();

public:
    const char* m_greeting;
    bool m_finish = false;
    bool m_result = false;
};
