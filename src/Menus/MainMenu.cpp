#include "MainMenu.h"

#include <assert.h>

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessList.h"

namespace Menu {

MainMenu::MainMenu() {
    auto printer = [](void* ctx, uint8_t id, uint8_t line) {
        gDisplay[line] << getActionName(static_cast<Action>(id));
    };

    auto maxGetter = [](void*) { return static_cast<uint8_t>(Action::last_); };

    m_listSelector = ListSelector(printer, maxGetter, this);
}

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
    m_listSelector.tick();

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
