#pragma once

#include "ProgDesc.h"

class Memory {
public:
    uint8_t getProgNum();
    ProgDesc getProg(uint8_t);
};
