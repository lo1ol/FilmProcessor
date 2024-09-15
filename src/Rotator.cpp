#include "Rotator.h"

#include <Arduino.h>

#include "Config.h"

void Rotator::start() {
    if (m_phase == Phase::Stop)
        m_servo.attach(SERVO);
    m_phase = Phase::Run;
}

void Rotator::stop() {
    if (m_phase != Phase::Run)
        return;
    m_phase = Phase::OnStop;
    if (m_pos > 90 && m_shift == 1)
        m_shift = -1;
    else if (m_pos < 90 && m_shift == -1)
        m_shift = 1;

    m_timer = millis();
}

void Rotator::tick() {
    if (m_phase == Phase::Stop)
        return;

    if (millis() < m_timer)
        return;

    if (m_pos == 88 && m_shift == 1)
        m_pos = 99;
    else if (m_pos == 99 && m_shift == -1)
        m_pos = 88;
    else
        m_pos += m_shift;

    uint16_t posTime = 50;
    if (m_pos == 126) {
        m_shift = -1;
        posTime = 1000;
    }
    if (m_pos == 60) {
        m_shift = 1;
        posTime = 1000;
    }

    m_servo.write(m_pos);

    if ((m_pos == 99 || m_pos == 88) && m_phase == Phase::OnStop) {
        m_servo.detach();
        m_phase = Phase::Stop;
        return;
    }

    m_timer = millis() + posTime;
}
