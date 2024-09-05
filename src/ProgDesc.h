#pragma once

#include <stdint.h>

class ProgDesc {
public:
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

    const char* name;
    Step steps[12];
};
