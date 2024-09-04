#pragma once

#include "../BaseMenu.h"
#include "../ListViewer.h"

namespace Menu {

class ProcessList : public BaseMenu {
public:
    ProcessList();
    void tick() override;

private:
    ListViewer m_listViewer;
};

} // namespace Menu
