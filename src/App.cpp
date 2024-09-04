#include "App.h"

void App::setMenu(BaseMenu* menu) {
    auto oldMenu = m_menu;
    m_menu = menu;

    if (oldMenu)
        delete (oldMenu);
}

void App::tick() {
    if (m_menu)
        m_menu->tick();
}
