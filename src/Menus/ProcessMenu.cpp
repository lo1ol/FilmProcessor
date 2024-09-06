#include "ProcessMenu.h"

#include <assert.h>

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessEdit.h"
#include "ProcessView.h"

namespace Menu {

ProcessMenu::ProcessMenu(const ProgDesc& progDesc) : m_progDesc(progDesc), m_listSelector((uint8_t)Action::last_) {}

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
        if (m_stringAsker.finish()) {
            ProgDesc newProg = m_progDesc;
            newProg.name = m_stringAsker.result();
            newProg.name.trim();
            gApp.setMenu(new ProcessEdit(newProg));
        }
        return;
    }

    m_listSelector.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_listSelector.low(); i != m_listSelector.high(); ++i, ++id) {
        gDisplay[id] << (m_listSelector.choosen(i) ? ">" : " ");
        gDisplay[id] << getActionName((Action)(i));
    }

    if (gModeSwitchBtn.click()) {
        switch ((Action)m_listSelector.pos()) {
        case Action::View:
            gApp.setMenu(new ProcessView(m_progDesc));
            return;
        case Action::Edit:
            gApp.setMenu(new ProcessEdit(m_progDesc));
            return;
        case Action::CreateBasedOn:
            m_onCreateNew = true;
            m_stringAsker = StringAsker("Name: ", m_progDesc.name);
            return;
        default:
            return;
        }
    }
}

} // namespace Menu
