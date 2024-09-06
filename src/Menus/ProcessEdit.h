#pragma once

#include "../BaseMenu.h"
#include "../ListSelector.h"
#include "../ProgDesc.h"

namespace Menu {

class ProcessEdit : public BaseMenu {
public:
    ProcessEdit(const ProgDesc&);
    void tick() override;

private:
    enum class Step { select, editAction, editTime, last_ };

    ProgDesc m_progDesc;
    ListSelector m_listSelector;
    Step m_step = Step::select;
    bool m_changeEnd = false;
};

} // namespace Menu
