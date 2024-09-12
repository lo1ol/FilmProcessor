#include <Arduino.h>

#include "Menus/MainMenu.h"
#include "Tools.h"

BaseMenu* gMenu;

void setup() {
    Serial.begin(9600);
    setupEncoder();

    Serial.println("Started");
    Serial.setTimeout(100);
    auto cmd = Serial.readString();

    if (cmd == "Loading") {
        gMemory.dump();
        gMemory.load();
    } else if (cmd == "Exporting") {
        gMemory.dump();
    }

    gApp.setMenu(new Menu::MainMenu());
}

void loop() {
    gEncoder.tick();
    gDisplay.tick();
    gModeSwitchBtn.tick();
    gBackBtn.tick();
    gApp.tick();
}
