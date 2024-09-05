#pragma once

#include "../BaseMenu.h"
#include "../ListViewer.h"
#include "../ProgDesc.h"

namespace Menu {

class ProcessEdit : public BaseMenu {
public:
    ProcessEdit(const ProgDesc&);
    void tick() override;

private:
    enum class Step { select, editAction, editTime, last_ };

    ProgDesc m_progDesc;
    ListViewer m_listViewer;
    Step m_step = Step::select;
    bool m_changeEnd = false;
};

} // namespace Menu
