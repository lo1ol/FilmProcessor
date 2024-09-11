#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"
#include "../ProgDesc.h"

namespace Menu {

class ProcessEdit : public BaseMenu {
public:
    ProcessEdit();

    void tick() override;

private:
    enum class Step { select, editAction, editTime, last_ };

    ListSelector m_listSelector;
    Step m_step = Step::select;
    ProgDesc::Action m_newAction;
};

} // namespace Menu
