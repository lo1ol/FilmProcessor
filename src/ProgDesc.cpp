#include "ProgDesc.h"

#include "Tools.h"

#include "MyAssert.h"

const char* ProgDesc::getActionName(Action action) {
    switch (action) {
    case Action::Dev:
        return "Dev";
    case Action::Bleach:
        return "Bleach";
    case Action::Fix:
        return "Fix";
    case Action::Dev2:
        return "Dev 2";
    case Action::ExtraBath:
        return "Extra";
    case Action::Wash:
        return "Wash";
    case Action::Wait:
        return "Wait";
    case Action::Finish:
        return "Finish";
    case Action::last_:
        break;
    }

    MyAssert(false);
}

bool ProgDesc::actionSupportTime(Action action) {
    switch (action) {
    case Action::Dev:
    case Action::Bleach:
    case Action::Fix:
    case Action::Dev2:
    case Action::ExtraBath:
    case Action::Wash:
        return true;
    case Action::Wait:
    case Action::Finish:
        return false;
    case Action::last_:
        break;
    }

    MyAssert(false);
}

uint8_t ProgDesc::numberOfSteps() const {
    for (uint8_t cnt = 0; cnt != ARRAY_SIZE(steps); ++cnt) {
        if (steps[cnt].action == Action::Finish)
            return cnt + 1;
    }

    MyAssert(false);
}

const char* ProgDesc::getStepName(uint8_t step) const {
    return getActionName(steps[step].action);
}

bool ProgDesc::stepSupportTime(uint8_t step) const {
    return actionSupportTime(steps[step].action);
}

void ProgDesc::copy(ProgDesc& target) const {
    memcpy(&target, this, sizeof(*this));
}
