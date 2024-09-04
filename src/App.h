#pragma once

#include "BaseMenu.h"

class App {
public:
    void setMenu(BaseMenu* menu);
    void tick();

private:
    BaseMenu* m_menu = nullptr;
};
