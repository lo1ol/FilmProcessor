#pragma once

#include "../BaseMenu.h"
#include "../ConfirmAsker.h"
#include "../MachineCleaner.h"

namespace Menu {

class MachineClean : public BaseMenu {
public:
    MachineClean();
    void tick() override;

private:
    MachineCleaner m_machineCleaner;
    ConfirmAsker m_confirmAsker;
    bool m_onAskConfirm = false;
};

} // namespace Menu
