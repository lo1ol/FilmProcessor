#include <Arduino.h>

#include "Menus/MainMenu.h"
#include "Tools.h"

BaseMenu* gMenu;

void setup() {
    Serial.begin(9600);
    pinMode(DEV_VALVE, OUTPUT);
    pinMode(FIX_VALVE, OUTPUT);
    pinMode(EXTRA_VALVE, OUTPUT);
    pinMode(WATER_VALVE, OUTPUT);
    pinMode(WASTE_VALVE, OUTPUT);
    pinMode(BLEACH_VALVE, OUTPUT);
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

    gMemory.getSettings().applySettings();

    gApp.setMenu(new Menu::MainMenu());
}

void loop() {
    gRotator.tick();
    gEncoder.tick();
    gDisplay.tick();
    gModeSwitchBtn.tick();
    gBackBtn.tick();
    gMelodyPlayer.tick();
    gApp.tick();
}
