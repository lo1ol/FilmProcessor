#pragma once

#include <Arduino.h>

class ConfirmAsker {
public:
    enum class Type {
        YesNo,
        ClickConfirm,
        HoldConfirm,
    };

    ConfirmAsker(const char* greeting = "", const char* question = "Are you sure?", Type type = Type::YesNo);
    bool finish() const { return m_finish; }
    bool result() const { return m_result; }

    void tick();

public:
    const char* m_greeting;
    const char* m_question;
    Type m_type;
    bool m_finish = false;
    bool m_result = false;
};
