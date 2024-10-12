#pragma once

#include <stdint.h>

#include <Servo.h>

class Rotator {
public:
    void tick();
    void start();
    void stop();

private:
    enum class Phase { Stop, Run, OnStop };

    int16_t m_pos = 90;
    int8_t m_shift = 1;
    Servo m_servo;
    Phase m_phase = Phase::Stop;
    uint32_t m_timer = 0;
};
