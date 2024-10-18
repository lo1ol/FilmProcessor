#pragma once

#include <stdint.h>

#include <Servo.h>
#include "ProcessSettings.h"

class Rotator {
public:
    void tick();
    void start();
    void stop();

private:
    void tickJobo();
    void tickCont();
    void tickOnStop();

    enum class Phase { Stop, Run, OnStop };

    int16_t m_pos = 90;
    int8_t m_shift = 1;
    Servo m_servo;
    Phase m_phase = Phase::Stop;
    uint32_t m_timer = 0;
    ProcessSettings::Agitation m_agitation;
};
