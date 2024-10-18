#include "ProcessSettings.h"

const char* ProcessSettings::getAgitationName(Agitation val) {
    switch (val) {
    case Agitation::Jobo:
        return "Jobo";
    case Agitation::Continious:
        return "Cont";
    default:
        return nullptr;
    }
}

const char* ProcessSettings::getVolumeName(Volume val) {
    switch (val) {
    case Volume::ml300:
        return "300ml";
    case Volume::ml400:
        return "400ml";
    case Volume::ml500:
        return "500ml";
    case Volume::ml600:
        return "600ml";
    case Volume::ml700:
        return "700ml";
    default:
        return nullptr;
    }
}

