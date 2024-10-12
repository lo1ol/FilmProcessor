#include "Rotator.h"

#include <Arduino.h>

#include "Config.h"

#define MAX_LEFT 0
#define MAX_RIGHT 180
#define SHIFT 3

void Rotator::start() {
    if (m_phase == Phase::Stop)
        m_servo.attach(SERVO);
    m_phase = Phase::Run;
}

void Rotator::stop() {
    if (m_phase != Phase::Run)
        return;
    m_phase = Phase::OnStop;
    if (m_pos > 90 && m_shift > 0)
        m_shift = -1 * SHIFT;
    else if (m_pos < 90 && m_shift < 0)
        m_shift = SHIFT;

    m_timer = millis();
}

void Rotator::tick() {
    if (m_phase == Phase::Stop)
        return;

    if (millis() < m_timer)
        return;

    m_pos += m_shift;

    uint16_t posTime = 1;
    if (m_pos <= 0) {
        m_pos = 0;
        m_shift = SHIFT;
        posTime = 5000;
    }
    if (m_pos >= 180) {
        m_pos = 180;
        m_shift = -1 * SHIFT;
        posTime = 5000;
    }

    m_servo.write(m_pos);

    if (abs(90 - m_pos) < 5 && m_phase == Phase::OnStop) {
        m_servo.detach();
        m_phase = Phase::Stop;
        return;
    }

    m_timer = millis() + posTime;
}
