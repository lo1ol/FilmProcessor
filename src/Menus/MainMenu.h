#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"

namespace Menu {

class MainMenu : public BaseMenu {
public:
    MainMenu();
    void tick() override;

private:
    enum class Action : uint8_t { ProcessList, LoadPrograms, DumpPrograms, last_ };

    static const char* getActionName(Action);

    ListSelector m_listSelector;
};

} // namespace Menu
