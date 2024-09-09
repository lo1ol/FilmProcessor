#pragma once

#include "Config.h"

#include <stdint.h>

class PageViewer {
public:
    PageViewer(uint8_t max);

    void shift(int8_t);
    uint8_t low() const { return m_low; }
    uint8_t high() const;

private:
    uint8_t m_max;
    uint8_t m_low = 0;
};
