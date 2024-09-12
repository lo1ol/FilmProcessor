#include "ProcessList.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MainMenu.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessList::ProcessList(uint8_t progId) {
    auto printer = [](void*, uint8_t id, uint8_t line) {
        ProgDesc progDesc;
        gMemory.getProg(id, progDesc);
        gDisplay[line] << progDesc.name;
    };
    auto maxGetter = [](void*) -> uint8_t { return gMemory.getProgNum(); };

    m_listSelector = ListSelector(printer, maxGetter, this, "Processes", progId);
}

void ProcessList::tick() {
    m_listSelector.shift(getEncoderDir());
    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        gMemory.setProg(m_listSelector.pos());
        gApp.setMenu(new ProcessMenu());
        return;
    }

    if (gBackBtn.click()) {
        gApp.setMenu(new MainMenu());
        return;
    }
}

} // namespace Menu
