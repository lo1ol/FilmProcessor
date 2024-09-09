#include <Arduino.h>

#include "Menus/MainMenu.h"
#include "Tools.h"

BaseMenu* gMenu;

void setup() {
    Serial.begin(9600);
    setupEncoder();
    gApp.setMenu(new Menu::MainMenu());
}

void loop() {
    gEncoder.tick();
    gDisplay.tick();
    gModeSwitchBtn.tick();
    gApp.tick();
}
