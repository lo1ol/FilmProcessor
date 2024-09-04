#include "ProcessList.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessList::ProcessList() : m_listViewer(gMemory.getProgNum()) {}

void ProcessList::tick() {
    m_listViewer.shift(getEncoderDir());
    for (uint8_t id = 0, i = m_listViewer.low(); i != m_listViewer.high(); ++i, ++id) {
        gDisplay[id] << (m_listViewer.choosen(i) ? ">" : " ");
        gDisplay[id] << gMemory.getProg(i).name;
    }

    if (gModeSwitchBtn.click())
        gApp.setMenu(new ProcessMenu(gMemory.getProg(m_listViewer.pos())));
}

} // namespace Menu
