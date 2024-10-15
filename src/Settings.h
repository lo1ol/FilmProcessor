#pragma once

#include "MelodyPlayer.h"

struct Settings {
    SongId songId;

    void applySettings() const;
};
