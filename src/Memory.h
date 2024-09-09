#pragma once

#include "ProgDesc.h"

class Memory {
public:
    Memory();

    uint8_t getProgNum() const;
    ProgDesc getProg(uint8_t) const;
    void saveProg(const ProgDesc&);

    void dump() const;
    void load();

private:
    uint8_t m_progNum;
};
