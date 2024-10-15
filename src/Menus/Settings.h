#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"

namespace Menu {

class Settings : public BaseMenu {
public:
    Settings();
    void tick() override;

private:
    enum class Phase { OnChoose, OnSet, last_ };
    enum class Setting { Agitation, Melody, last_ };

    static const char* getSettingName(Setting);
    static const char* getSettingValue(Setting);

    ListSelector m_listSelector;
    Phase m_phase = Phase::OnChoose;
    Setting m_setting = Setting::Agitation;
};

} // namespace Menu
