#pragma once

#include <Config.h>

#include <stdint.h>

class ListViewer {
public:
    ListViewer(uint8_t max, bool pageView = false, uint8_t pos = 0);

    void shift(int8_t);
    uint8_t low() const { return m_low; }
    uint8_t high() const;
    uint8_t pos() const { return m_pos; }
    bool choosen(uint8_t pos) const { return m_pos == pos; }

private:
    const uint8_t m_max;
    uint8_t m_low;
    uint8_t m_pos;
    bool m_pageView;
};
