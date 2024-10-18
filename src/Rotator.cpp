#include "Rotator.h"

#include <Arduino.h>

#include "Config.h"
#include "Tools.h"

#define MAX_LEFT 0
#define MAX_RIGHT 180
#define SHIFT 3

void Rotator::start() {
    if (m_phase == Phase::Stop)
        m_servo.attach(SERVO);
    m_phase = Phase::Run;
    m_agitation = gMemory.getSettings().lastProcessSettings.agitation;
}

void Rotator::stop() {
    if (m_phase != Phase::Run)
        return;
    m_phase = Phase::OnStop;
    tickOnStop();
}

void Rotator::tick() {
    if (m_phase == Phase::Stop)
        return;

    if (millis() < m_timer)
        return;

    m_pos += m_shift;
    m_servo.write(m_pos);

    if (m_phase == Phase::OnStop) {
        tickOnStop();
        return;
    }

    switch (m_agitation) {
    case ProcessSettings::Agitation::Jobo:
        tickJobo();
        break;
    case ProcessSettings::Agitation::Continious:
        tickCont();
        break;
    default:
        MyAssert(false);
    }
}

void Rotator::tickJobo() {
    uint16_t posTime = 1;
    if (m_pos <= 0) {
        m_pos = 0;
        m_shift = SHIFT;
        posTime = 2000;
    }
    if (m_pos >= 180) {
        m_pos = 180;
        m_shift = -1 * SHIFT;
        posTime = 2000;
    }

    m_timer = millis() + posTime;
}

void Rotator::tickCont() {
    uint16_t posTime = 1;

    m_shift = SHIFT;
    if (m_pos >= 180) {
        m_pos = 180;
        m_shift = 0;
        posTime = 1000;
    }

    m_timer = millis() + posTime;
}

void Rotator::tickOnStop() {
    if (abs(90 - m_pos) < 5) {
        m_servo.detach();
        m_phase = Phase::Stop;
        return;
    }

    if (m_pos > 90)
        m_shift = -1 * SHIFT;
    else
        m_shift = SHIFT;

    m_timer = millis() + 1;
}
