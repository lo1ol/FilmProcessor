#include "ProcessList.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MainMenu.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessList::ProcessList(uint8_t progId) {
    auto printer = [](void*, uint8_t id, uint8_t line) {
        if (id == gMemory.getProgNum()) {
            gDisplay[line] << "Add new one...";
        }
        ProgDesc progDesc;
        gMemory.getProg(id, progDesc);
        gDisplay[line] << progDesc.name;
    };
    auto maxGetter = [](void*) -> uint8_t { return gMemory.getProgNum() + 1; };

    m_listSelector = ListSelector(printer, maxGetter, this, "Processes", progId);
}

void ProcessList::tick() {
    if (gBackBtn.click()) {
        if (m_phase == Phase::OnAddNew) {
            m_phase = Phase::OnChoose;
            return;
        }
        gApp.setMenu(new MainMenu());
        return;
    }

    if (m_phase == Phase::OnAddNew) {
        m_stringAsker.tick();

        if (m_stringAsker.finish()) {
            ProgDesc newProg;

            strcpy(newProg.name, m_stringAsker.result());
            gMemory.setProg(newProg);
            gMemory.saveProg();
            gApp.setMenu(new ProcessMenu());
        }

        return;
    }

    m_listSelector.shift(getEncoderDir());
    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        auto pos = m_listSelector.pos();

        if (pos == gMemory.getProgNum()) {
            m_phase = Phase::OnAddNew;
            m_stringAsker = StringAsker("Name: ", "");
            return;
        }

        gMemory.setProg(pos);
        gApp.setMenu(new ProcessMenu());
        return;
    }
}

} // namespace Menu
