#pragma once

#include <stdint.h>

struct ProcessSettings {
    enum class Agitation : uint8_t { Jobo, last_ };
    enum class Volume : uint32_t { ml300 = 300, ml400 = 400, ml500 = 500, ml600 = 600, ml700 = 700, last_ };

    static const char* getAgitationName(Agitation);
    static const char* getVolumeName(Volume);

    Agitation agitation;
    Volume volume;
};
