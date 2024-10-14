#pragma once

#include <stdint.h>

class Melody {
public:
    const char* name;
    int16_t notes[32];
    int8_t durations[32];
};

class MelodyPlayer {
public:
    MelodyPlayer();

    void start();
    void stop();
    bool end() const { return !m_play; }

    static Melody* getMelodies();

    void tick();

private:
    void playPhase();

    const Melody* m_melody;
    uint8_t m_melodyPhase;
    bool m_play = false;
    uint32_t m_timer;
};
