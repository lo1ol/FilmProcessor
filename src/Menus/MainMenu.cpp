#include "MainMenu.h"

#include <assert.h>

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessList.h"

namespace Menu {

MainMenu::MainMenu() : m_listSelector((uint8_t)Action::last_) {}

const char* MainMenu::getActionName(Action action) {
    switch (action) {
    case Action::ProcessList:
        return "Process list";
    case Action::LoadPrograms:
        return "Load programs";
    case Action::DumpPrograms:
        return "Dump programs";
    case Action::last_:
        assert(false);
    }

    assert(false);
    return nullptr;
}

void MainMenu::tick() {
    m_listSelector.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_listSelector.low(); i != m_listSelector.high(); ++i, ++id) {
        gDisplay[id] << (m_listSelector.choosen(i) ? ">" : " ");
        gDisplay[id] << getActionName((Action)(i));
    }

    if (gModeSwitchBtn.click()) {
        switch ((Action)m_listSelector.pos()) {
        case Action::ProcessList:
            gApp.setMenu(new ProcessList());
            return;
        case Action::LoadPrograms:
            gMemory.load();
            return;
        default:
            gMemory.dump();
            return;
        }
    }
}

} // namespace Menu
