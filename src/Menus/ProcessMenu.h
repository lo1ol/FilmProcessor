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
    enum class Action : uint8_t { Process, View, Edit, CreateBasedOn, Delete, last_ };

    static const char* getActionName(Action);

    ProgDesc m_progDesc;
    ListViewer m_listViewer;
};

} // namespace Menu
