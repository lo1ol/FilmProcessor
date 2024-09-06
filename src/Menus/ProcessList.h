#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"

namespace Menu {

class ProcessList : public BaseMenu {
public:
    ProcessList();
    void tick() override;

private:
    ListSelector m_listSelector;
};

} // namespace Menu
