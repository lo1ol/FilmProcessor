#include "ListSelector.h"

#include <Arduino.h>

ListSelector::ListSelector(uint8_t max, uint8_t pos) : m_max(max), m_pos(pos) {}

void ListSelector::shift(int8_t shift_) {
    if (shift_ < 0 && m_pos < -shift_) {
        m_low = 0;
        m_pos = 0;
    } else if (m_pos + shift_ >= m_max) {
        m_low = max(0, int(m_max) - DISPLAY_ROWS);
        m_pos = max(0, m_max - 1);
    } else {
        m_pos += shift_;

        if (m_pos == m_low && m_low > 0)
            m_low = m_pos - 1;

        if (m_pos == m_low + DISPLAY_ROWS - 1 && m_low + DISPLAY_ROWS < m_max)
            m_low = m_pos - DISPLAY_ROWS + 2;

        if (m_pos < m_low)
            m_low = m_pos;

        if (m_pos >= m_low + DISPLAY_ROWS)
            m_low = m_pos - DISPLAY_ROWS;
    }
}

uint8_t ListSelector::high() const {
    return min(m_max, m_low + DISPLAY_ROWS);
}

void ListSelector::setMax(uint8_t max) {
    m_max = max;
    if (m_pos > m_max) {
        if (m_max)
            m_pos = m_max - 1;
        else
            m_pos = 0;
    }

    if (m_low + DISPLAY_ROWS > m_max) {
        if (m_max < DISPLAY_ROWS)
            m_low = 0;
        else
            m_low = m_max - DISPLAY_ROWS;
    }
}
