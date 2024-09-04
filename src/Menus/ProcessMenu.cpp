#include "ProcessMenu.h"

#include <assert.h>

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessView.h"

namespace Menu {

ProcessMenu::ProcessMenu(const ProgDesc& progDesc) : m_progDesc(progDesc), m_listViewer((uint8_t)Action::last_) {}

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
    m_listViewer.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_listViewer.low(); i != m_listViewer.high(); ++i, ++id) {
        gDisplay[id] << (m_listViewer.choosen(i) ? ">" : " ");
        gDisplay[id] << getActionName((Action)i);
    }

    if (gModeSwitchBtn.click()) {
        switch ((Action)m_listViewer.pos()) {
        case Action::View:
            gApp.setMenu(new ProcessView(m_progDesc));
            break;
        default:
            return;
        }
    }
}

} // namespace Menu
