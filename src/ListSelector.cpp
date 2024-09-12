#include "ListSelector.h"

#include <Arduino.h>

#include "Tools.h"

ListSelector::ListSelector(Printer printer, MaxGetter maxGetter, void* ctx, const char* header)
    : m_printer(printer), m_maxGetter(maxGetter), m_ctx(ctx), m_header(header) {}

void ListSelector::shift(int8_t shift_) {
    int8_t minIndex = m_header ? -1 : 0;

    m_pos += shift_;

    int max_ = m_maxGetter(m_ctx);
    if (m_pos < minIndex) {
        m_low = minIndex;
        m_pos = minIndex;
    }
    if (m_pos >= max_) {
        m_low = max(minIndex, max_ - DISPLAY_ROWS);
        m_pos = max(minIndex, max_ - 1);
    }

    if (m_pos == m_low && m_low > minIndex)
        m_low = m_pos - 1;

    if (m_pos == m_low + DISPLAY_ROWS - 1 && m_low + DISPLAY_ROWS < max_)
        m_low = m_pos - DISPLAY_ROWS + 2;

    if (m_pos < m_low)
        m_low = m_pos;

    if (m_pos >= m_low + DISPLAY_ROWS)
        m_low = m_pos - DISPLAY_ROWS;

    if (m_pos == -1)
        m_pos = 0;
}

void ListSelector::tick() {
    auto max = m_maxGetter(m_ctx);
    int8_t i = m_low;
    for (uint8_t line = 0; line != DISPLAY_ROWS; ++i, ++line) {
        if (i == max)
            return;
        if (i == -1) {
            gDisplay[line].printHeader(m_header);
            continue;
        }
        gDisplay[line] << (i == m_pos ? ">" : " ");
        m_printer(m_ctx, i, line);
    }
}
