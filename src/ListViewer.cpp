#include <ListViewer.h>

#include <Arduino.h>

ListViewer::ListViewer(uint8_t max, uint8_t frameSize, uint8_t pos) : m_max(max), m_frameSize(frameSize), m_pos(pos) {}

bool ListViewer::shift(int8_t shift_) {
    if (shift_ < 0 && m_pos < -shift_) {
        m_low = 0;
        m_pos = 0;
        return true;
    }

    if (m_pos + shift_ + 1 >= m_max) {
        m_low = max(0, int(m_max) - m_frameSize);
        m_pos = max(0, m_max - 1);
        return true;
    }

    m_pos += shift_;

    if (m_pos < m_low)
        m_low = m_pos;

    if (m_pos > (m_low + m_frameSize))
        m_low = m_pos - m_frameSize;

    return false;
}

uint8_t ListViewer::high() const {
    return min(m_max, m_low + m_frameSize);
}
