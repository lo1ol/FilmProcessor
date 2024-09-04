#pragma once

#include "../BaseMenu.h"
#include "../ListViewer.h"
#include "../ProgDesc.h"

namespace Menu {

class ProcessMenu : public BaseMenu {
public:
    ProcessMenu(const ProgDesc&);
    void tick() override;

private:
    ProgDesc m_progDesc;
    ListViewer m_listViewer;
};

} // namespace Menu
