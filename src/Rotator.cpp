#include "Rotator.h"

#include <Arduino.h>

#include "Config.h"

#define DEAD_LEFT 60
#define DEAD_RIGHT 130
#define MAX_LEFT 59
#define MAX_RIGHT 131

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

    if (m_pos == DEAD_LEFT && m_shift == 1)
        m_pos = DEAD_RIGHT;
    else if (m_pos == DEAD_RIGHT && m_shift == -1)
        m_pos = DEAD_LEFT;
    else
        m_pos += m_shift;

    uint16_t posTime = 20;
    if (m_pos == MAX_LEFT) {
        m_shift = 1;
        posTime = 3000;
    }
    if (m_pos == MAX_RIGHT) {
        m_shift = -1;
        posTime = 3000;
    }

    m_servo.write(m_pos);

    if ((m_pos == DEAD_RIGHT || m_pos == DEAD_LEFT) && m_phase == Phase::OnStop) {
        m_servo.detach();
        m_phase = Phase::Stop;
        return;
    }

    m_timer = millis() + posTime;
}
