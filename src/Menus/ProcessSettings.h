#pragma once

#include "../BaseMenu.h"
#include "../ConfirmAsker.h"
#include "../ListSelector.h"
#include "../ProcessSettings.h"

namespace Menu {

class ProcessSettings : public BaseMenu {
public:
    ProcessSettings();
    void tick() override;

private:
    enum class Phase { OnChoose, OnSet, OnStart, last_ };
    enum class Setting { Agitation, Volume, Start, last_ };

    static const char* getSettingName(Setting);
    const char* getSettingValue(Setting);
    void changeValue(Setting setting, int8_t shift);

    Phase m_phase = Phase::OnChoose;
    ListSelector m_listSelector;
    ConfirmAsker m_confirmAsker;
    ::ProcessSettings m_processSettings;
};

} // namespace Menu
