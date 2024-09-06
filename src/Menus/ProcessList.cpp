#include "ProcessList.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessList::ProcessList() : m_listSelector(gMemory.getProgNum()) {}

void ProcessList::tick() {
    m_listSelector.shift(getEncoderDir());
    for (uint8_t id = 0, i = m_listSelector.low(); i != m_listSelector.high(); ++i, ++id) {
        gDisplay[id] << (m_listSelector.choosen(i) ? ">" : " ");
        gDisplay[id] << gMemory.getProg(i).name;
    }

    if (gModeSwitchBtn.click())
        gApp.setMenu(new ProcessMenu(gMemory.getProg(m_listSelector.pos())));
}

} // namespace Menu
