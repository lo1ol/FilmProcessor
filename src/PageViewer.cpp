#include "PageViewer.h"

#include <Arduino.h>

PageViewer::PageViewer(Printer printer, MaxGetter maxGetter, void* ctx)
    : m_printer(printer), m_maxGetter(maxGetter), m_ctx(ctx) {}

void PageViewer::tick() {
    auto max = m_maxGetter(m_ctx);
    for (uint8_t line = 0, i = m_low; line != DISPLAY_ROWS; ++i, ++line) {
        if (i == max)
            return;
        m_printer(m_ctx, i, line);
    }
}
void PageViewer::shift(int8_t shift_) {
    int max_ = m_maxGetter(m_ctx);
    if (shift_ < 0 && m_low < -shift_) {
        m_low = 0;
    } else if (m_low + shift_ + DISPLAY_ROWS >= max_) {
        m_low = max(0, int(max_) - DISPLAY_ROWS);
    } else {
        m_low += shift_;
    }
}
