#pragma once

#include "../BaseMenu.h"
#include "../MachineCleaner.h"

namespace Menu {

class MachineClean : public BaseMenu {
public:
    MachineClean();
    void tick() override;

private:
    MachineCleaner m_machineCleaner;
};

} // namespace Menu
