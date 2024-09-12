#include "ProcessMenu.h"

#include <assert.h>

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessEdit.h"
#include "ProcessList.h"
#include "ProcessView.h"

namespace Menu {

ProcessMenu::ProcessMenu() {
    auto printer = [](void*, uint8_t id, uint8_t line) { gDisplay[line] << getActionName(static_cast<Action>(id)); };

    auto maxGetter = [](void*) { return static_cast<uint8_t>(Action::last_); };

    m_listSelector = ListSelector(printer, maxGetter, this, gMemory.getProg().name);
}

const char* ProcessMenu::getActionName(Action action) {
    switch (action) {
    case Action::Process:
        return "Process";
    case Action::Edit:
        return "Edit";
    case Action::View:
        return "View";
    case Action::CreateBasedOn:
        return "Create based on";
    case Action::Delete:
        return "Delete";
    case Action::last_:
        assert(false);
    }

    assert(false);
    return nullptr;
}

void ProcessMenu::tick() {
    if (m_onCreateNew) {
        m_stringAsker.tick();

        if (gBackBtn.click()) {
            m_onCreateNew = false;
        }

        if (m_stringAsker.finish()) {
            ProgDesc newProg;
            gMemory.getProg().copy(newProg);

            strcpy(newProg.name, m_stringAsker.result());
            gMemory.setProg(newProg);
            gApp.setMenu(new ProcessEdit());
        }

        return;
    }

    if (m_onDelete) {
        m_conirmAsker.tick();
        if (m_conirmAsker.finish()) {
            if (m_conirmAsker.result()) {
                gMemory.deleteProg();
                gApp.setMenu(new ProcessList());
            } else {
                m_onDelete = false;
            }
        }
        return;
    }

    m_listSelector.shift(getEncoderDir());
    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        switch ((Action)m_listSelector.pos()) {
        case Action::View:
            gApp.setMenu(new ProcessView());
            return;
        case Action::Edit:
            gApp.setMenu(new ProcessEdit());
            return;
        case Action::CreateBasedOn:
            m_onCreateNew = true;
            m_stringAsker = StringAsker("Name: ", gMemory.getProg().name);
            return;
        case Action::Delete:
            m_onDelete = true;
            m_conirmAsker = ConfirmAsker();
            return;
        default:
            return;
        }
    }

    if (gBackBtn.click()) {
        gApp.setMenu(new ProcessList(gMemory.progId()));
        return;
    }
}

} // namespace Menu
