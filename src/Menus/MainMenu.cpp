#include "MainMenu.h"

#include "MyAssert.h"

#include "../Memory.h"
#include "../Tools.h"
#include "MachineClean.h"
#include "ProcessList.h"

namespace Menu {

MainMenu::MainMenu() {
    auto printer = [](void* ctx, uint8_t id, uint8_t line) {
        gDisplay[line] << getActionName(static_cast<Action>(id));
    };

    auto maxGetter = [](void*) { return static_cast<uint8_t>(Action::last_); };

    m_listSelector = ListSelector(printer, maxGetter, this, "Menu");
}

const char* MainMenu::getActionName(Action action) {
    switch (action) {
    case Action::ProcessList:
        return "Process list";
    case Action::CleanMachine:
        return "Clean machine";
    case Action::last_:
        MyAssert(false);
    }

    MyAssert(false);
    return nullptr;
}

void MainMenu::tick() {
    if (gBackBtn.click() && m_phase != Phase::OnChoose) {
        m_phase = Phase::OnChoose;
        return;
    }

    if (m_phase == Phase::OnCleanMachine) {
        m_conirmAsker.tick();
        if (m_conirmAsker.finish()) {
            if (m_conirmAsker.result())
                gApp.setMenu(new MachineClean());
            else
                m_phase = Phase::OnChoose;
        }
        return;
    }


    m_listSelector.shift(getEncoderDir());
    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        switch ((Action)m_listSelector.pos()) {
        case Action::ProcessList:
            gApp.setMenu(new ProcessList());
            return;
        case Action::CleanMachine:
            m_phase = Phase::OnCleanMachine;
            m_conirmAsker = ConfirmAsker("Clean machine");
            return;
        case Action::last_:
            MyAssert(false);
        }
    }
}

} // namespace Menu
