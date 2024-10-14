#include "ProcessMenu.h"

#include "../MyAssert.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessEdit.h"
#include "ProcessExecutor.h"
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
        MyAssert(false);
    }

    MyAssert(false);
    return nullptr;
}

void ProcessMenu::tick() {
    if (gBackBtn.click()) {
        if (m_phase == Phase::OnChoose)
            gApp.setMenu(new ProcessList(gMemory.progId()));
        else
            m_phase = Phase::OnChoose;
        return;
    }

    if (m_phase == Phase::OnCreateNew) {
        m_stringAsker.tick();

        if (m_stringAsker.finish()) {
            ProgDesc newProg;
            gMemory.getProg().copy(newProg);

            strcpy(newProg.name, m_stringAsker.result());
            gMemory.setProg(newProg);
            gMemory.saveProg();
            gApp.setMenu(new ProcessEdit());
        }

        return;
    }

    if (m_phase == Phase::OnDelete || m_phase == Phase::OnProcessRun) {
        m_conirmAsker.tick();
        if (m_conirmAsker.finish()) {
            if (m_conirmAsker.result()) {
                if (m_phase == Phase::OnDelete) {
                    gMemory.deleteProg();
                    gApp.setMenu(new ProcessList());
                } else {
                    gApp.setMenu(new ProcessExecutor());
                }
            } else {
                m_phase = Phase::OnChoose;
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
            m_phase = Phase::OnCreateNew;
            m_stringAsker = StringAsker("Name: ", gMemory.getProg().name);
            return;
        case Action::Delete:
            m_phase = Phase::OnDelete;
            m_conirmAsker = ConfirmAsker("Delete process");
            return;
        case Action::Process:
            m_phase = Phase::OnProcessRun;
            m_conirmAsker = ConfirmAsker("Run process");
            return;
        case Action::last_:
            MyAssert(false);
        }
    }
}

} // namespace Menu
