#include <Arduino.h>

#include "Menus/ProcessList.h"
#include "Tools.h"

BaseMenu* gMenu;

void setup() {
    setupEncoder();
    gApp.setMenu(new Menu::ProcessList());
}

void loop() {
    gEncoder.tick();
    gDisplay.tick();
    gModeSwitchBtn.tick();
    gApp.tick();
}
