#include "ListSelector.h"

#include <Arduino.h>

#include "Tools.h"

ListSelector::ListSelector(Printer printer, MaxGetter maxGetter, void* ctx)
    : m_printer(printer), m_maxGetter(maxGetter), m_ctx(ctx) {}

void ListSelector::shift(int8_t shift_) {
    int max_ = m_maxGetter(m_ctx);
    if (shift_ < 0 && m_pos < -shift_) {
        m_low = 0;
        m_pos = 0;
    } else if (m_pos + shift_ >= max_) {
        m_low = max(0, max_ - DISPLAY_ROWS);
        m_pos = max(0, max_ - 1);
    } else {
        m_pos += shift_;

        if (m_pos == m_low && m_low > 0)
            m_low = m_pos - 1;

        if (m_pos == m_low + DISPLAY_ROWS - 1 && m_low + DISPLAY_ROWS < max_)
            m_low = m_pos - DISPLAY_ROWS + 2;

        if (m_pos < m_low)
            m_low = m_pos;

        if (m_pos >= m_low + DISPLAY_ROWS)
            m_low = m_pos - DISPLAY_ROWS;
    }
}

void ListSelector::tick() {
    auto max = m_maxGetter(m_ctx);
    for (uint8_t line = 0, i = m_low; line != DISPLAY_ROWS; ++i, ++line) {
        if (i == max)
            return;
        gDisplay[line] << (i == m_pos ? ">" : " ");
        m_printer(m_ctx, i, line);
    }
}
