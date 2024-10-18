#pragma once

#include "../BaseMenu.h"
#include "../ConfirmAsker.h"
#include "../ListSelector.h"
#include "../ProgDesc.h"
#include "../StringAsker.h"

namespace Menu {

class ProcessMenu : public BaseMenu {
public:
    ProcessMenu();
    void tick() override;

private:
    enum class Action : uint8_t { Process, View, Edit, CreateBasedOn, Delete, last_ };
    enum class Phase : uint8_t { OnChoose, OnDelete, OnCreateNew };

    static const char* getActionName(Action);

    ListSelector m_listSelector;
    Phase m_phase = Phase::OnChoose;
    StringAsker m_stringAsker;
    ConfirmAsker m_conirmAsker;
};

} // namespace Menu
