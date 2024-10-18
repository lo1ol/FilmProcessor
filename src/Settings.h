#pragma once

#include "MelodyPlayer.h"
#include "ProcessSettings.h"

struct Settings {
    SongId songId;
    ProcessSettings lastProcessSettings;

    void applySettings() const;
};
