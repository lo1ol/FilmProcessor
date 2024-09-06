#include <PageViewer.h>

#include <Arduino.h>

PageViewer::PageViewer(uint8_t max) : m_max(max) {}

void PageViewer::shift(int8_t shift_) {
    if (shift_ < 0 && m_low < -shift_) {
        m_low = 0;
    } else if (m_low + shift_ + DISPLAY_ROWS >= m_max) {
        m_low = max(0, int(m_max) - DISPLAY_ROWS);
    } else {
        m_low += shift_;
    }
}

uint8_t PageViewer::high() const {
    return min(m_max, m_low + DISPLAY_ROWS);
}
