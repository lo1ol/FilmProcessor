#pragma once

#include "Config.h"

#include <stdint.h>

class ListSelector {
public:
    ListSelector(uint8_t max, uint8_t pos = 0);

    void shift(int8_t);
    uint8_t low() const { return m_low; }
    uint8_t high() const;
    uint8_t pos() const { return m_pos; }
    bool choosen(uint8_t pos) const { return m_pos == pos; }
    void setMax(uint8_t max);

private:
    uint8_t m_max;
    uint8_t m_low;
    uint8_t m_pos;
};
