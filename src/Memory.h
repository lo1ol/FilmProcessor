#pragma once

#include "ProgDesc.h"

class Memory {
public:
    Memory();

    uint8_t getProgNum() const;
    void getProg(uint8_t, ProgDesc&) const;

    // utils to cache one program
    void setProg(uint8_t id);
    uint8_t progId();
    void setProg(const ProgDesc& prog) { prog.copy(m_cachedProg); }
    ProgDesc& getProg() { return m_cachedProg; }
    void saveProg();
    void deleteProg();

    void dump() const;
    void load();

private:
    uint8_t m_progNum;
    ProgDesc m_cachedProg;
};
