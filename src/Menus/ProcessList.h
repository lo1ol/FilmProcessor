#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"
#include "../StringAsker.h"

namespace Menu {

class ProcessList : public BaseMenu {
public:
    ProcessList(uint8_t progId = 0);
    void tick() override;

private:
    enum class Phase { OnChoose, OnAddNew };

    Phase m_phase = Phase::OnChoose;
    ListSelector m_listSelector;
    StringAsker m_stringAsker;
};

} // namespace Menu
