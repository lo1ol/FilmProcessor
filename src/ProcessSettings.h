#pragma once

struct ProcessSettings {
    enum class Agitation { Jobo, last_ };

    enum class Volume { ml300 = 300, ml400 = 400, ml500 = 500, ml600 = 600, ml700 = 700, last_ };

    Agitation agitation;
    Volume volume;
};
