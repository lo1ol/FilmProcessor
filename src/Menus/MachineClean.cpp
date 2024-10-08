#include "MachineClean.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MainMenu.h"

namespace Menu {

MachineClean::MachineClean() {}

void MachineClean::tick() {
    if (m_onAskConfirm) {
        m_confirmAsker.tick();
        if (!m_confirmAsker.finish())
            return;
        m_onAskConfirm = false;
        m_machineCleaner.nextPhase();
    }

    m_machineCleaner.tick();

    if (m_machineCleaner.needConfirm()) {
        m_confirmAsker = ConfirmAsker(m_machineCleaner.confirmName(), "", ConfirmAsker::Type::HoldConfirm);
        m_onAskConfirm = true;
    }

    if (m_machineCleaner.finish()) {
        gApp.setMenu(new MainMenu);
        return;
    }

    gDisplay[0].printHeader("Cleaning");
    gDisplay[1] << m_machineCleaner.stepName();
    gDisplay[2].printTimeProgress(m_machineCleaner.totalTime(), m_machineCleaner.passedTime(), true);
}

} // namespace Menu
