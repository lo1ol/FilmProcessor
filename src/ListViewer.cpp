#include <ListViewer.h>

#include <Arduino.h>

ListViewer::ListViewer(uint8_t max, bool pageView, uint8_t pos) : m_max(max), m_pos(pos), m_pageView(pageView) {}

void ListViewer::shift(int8_t shift_) {
    auto adjustForPageView = [this] {
        if (!m_pageView)
            return;

        if (m_max <= DISPLAY_ROWS) {
            m_pos = 0;
            return;
        }

        if (m_pos + DISPLAY_ROWS >= m_max)
            m_pos = m_max - DISPLAY_ROWS;

        m_low = m_pos;
    };

    if (shift_ < 0 && m_pos < -shift_) {
        m_low = 0;
        m_pos = 0;
        return;
    }

    if (m_pos + shift_ + 1 >= m_max) {
        m_low = max(0, int(m_max) - DISPLAY_ROWS);
        m_pos = max(0, m_max - 1);
        adjustForPageView();
        return;
    }

    m_pos += shift_;

    if (m_pos < m_low)
        m_low = m_pos;

    if (m_pos > (m_low + DISPLAY_ROWS))
        m_low = m_pos - DISPLAY_ROWS;

    if (m_pageView) {
        adjustForPageView();
        return;
    }

    if (shift_ >= 0 && m_pos != m_max && m_pos == m_low + DISPLAY_ROWS - 1)
        ++m_low;
    if (shift_ < 0 && m_pos != 0 && m_pos == m_low)
        --m_low;
}

uint8_t ListViewer::high() const {
    return min(m_max, m_low + DISPLAY_ROWS);
}

void ListViewer::setMax(uint8_t max) {
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
