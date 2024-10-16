#pragma once

#include <stdint.h>

#include <Arduino.h>

class ProgDesc {
public:
    ProgDesc();
    ProgDesc(const ProgDesc&) = delete;
    ProgDesc(ProgDesc&&) = delete;
    ProgDesc& operator=(const ProgDesc&) = delete;
    ProgDesc& operator=(ProgDesc&&) = delete;

    enum class Action {
        Dev,
        Bleach,
        Fix,
        Dev2,
        ExtraBath,
        Wash,
        Wait,
        Finish,
        last_,
    };

    struct Step {
        Action action;
        uint16_t time;
    };

    uint8_t numberOfSteps() const;
    const char* getStepName(uint8_t step) const;
    bool stepSupportTime(uint8_t step) const;
    static const char* getActionName(Action action);
    static bool actionSupportTime(Action action);

    void copy(ProgDesc&) const;

    char name[11];
    Step steps[12];
};
