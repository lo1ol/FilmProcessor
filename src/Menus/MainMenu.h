#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"
#include "../ConfirmAsker.h"

namespace Menu {

class MainMenu : public BaseMenu {
public:
    MainMenu();
    void tick() override;

private:
    enum class Action : uint8_t { ProcessList, CleanMachine, last_ };
    enum class Phase: uint8_t { OnChoose, OnCleanMachine };

    static const char* getActionName(Action);

    ListSelector m_listSelector;
    Phase m_phase = Phase::OnChoose;
    ConfirmAsker m_conirmAsker;
};

} // namespace Menu
