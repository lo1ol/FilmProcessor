#include "MachineClean.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MainMenu.h"

namespace Menu {

MachineClean::MachineClean() {}

void MachineClean::tick() {
    m_machineCleaner.tick();

    if (m_machineCleaner.finish()) {
        gApp.setMenu(new MainMenu);
        return;
    }

    gDisplay[0].printHeader("Cleaning");
    gDisplay[1] << m_machineCleaner.stepName();
    gDisplay[2].printTimeProgress(m_machineCleaner.totalTime(), m_machineCleaner.passedTime(), true);
}

} // namespace Menu
