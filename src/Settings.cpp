#include "Settings.h"

#include "Tools.h"

void Settings::applySettings() const {
    gMelodyPlayer.setSong(songId);
}
