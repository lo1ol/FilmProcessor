#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"
#include "../Settings.h"

namespace Menu {

class Settings : public BaseMenu {
public:
    Settings();
    void tick() override;

private:
    enum class Phase { OnChoose, OnSet, last_ };
    enum class Setting { Melody, last_ };

    static const char* getSettingName(Setting);
    const char* getSettingValue(Setting);
    void changeValue(Setting setting, int8_t shift);

    ListSelector m_listSelector;
    Phase m_phase = Phase::OnChoose;
    Setting m_setting = Setting::Melody;
    ::Settings m_settings;
};

} // namespace Menu
