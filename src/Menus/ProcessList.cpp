#include "ProcessList.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MainMenu.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessList::ProcessList() {
    auto printer = [](void*, uint8_t id, uint8_t line) { gDisplay[line] << gMemory.getProg(id).name; };
    auto maxGetter = [](void*) -> uint8_t { return gMemory.getProgNum(); };

    m_listSelector = ListSelector(printer, maxGetter, this);
}

void ProcessList::tick() {
    m_listSelector.shift(getEncoderDir());
    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        gApp.setMenu(new ProcessMenu(gMemory.getProg(m_listSelector.pos())));
        return;
    }

    if (gBackBtn.click()) {
        gApp.setMenu(new MainMenu());
        return;
    }
}

} // namespace Menu
