#include "ProgDesc.h"

#include "Tools.h"

#include <assert.h>

uint8_t ProgDesc::numberOfSteps() const {
    uint8_t cnt = 0;
    for (; cnt != ARRAY_SIZE(steps); ++cnt) {
        if (steps[cnt].action == Action::Finish)
            return cnt + 1;
    }

    assert(false);
}

const char* ProgDesc::getStepName(uint8_t step) const {
    switch (steps[step].action) {
    case Action::Dev:
        return "Dev";
    case Action::Bleach:
        return "Bleach";
    case Action::Fix:
        return "Fix";
    case Action::Dev2:
        return "Dev 2";
    case Action::ExtraBath:
        return "Extra bath";
    case Action::Wash:
        return "Wash";
    case Action::Wait:
        return "Wait";
    case Action::Finish:
        return "Finish";
    }

    assert(false);
}

