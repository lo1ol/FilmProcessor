#pragma once

#include <stdint.h>

enum class SongId : uint8_t { Simpsons, Pacman, last_ };

class Melody {
public:
    static const char* getSongName(SongId);
    SongId id;
    const uint16_t* notes;
    const uint8_t* durations;
};

class MelodyPlayer {
public:
    MelodyPlayer();

    void setSong(SongId);

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
