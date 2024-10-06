#include "Pump.h"

#include <Arduino.h>

#include "Config.h"
#include "Tools.h"

Pump::Pump() {
    pinMode(PUMP_FOR, OUTPUT);
    pinMode(PUMP_REV, OUTPUT);
}

void Pump::loadInTank() {
    digitalWrite(PUMP_FOR, HIGH);
    digitalWrite(PUMP_REV, LOW);
    delay(10);
    gDisplay.reinitDisplay();
}

void Pump::loadOutTank() {
    digitalWrite(PUMP_FOR, LOW);
    digitalWrite(PUMP_REV, HIGH);
    delay(10);
    gDisplay.reinitDisplay();
}

void Pump::stop() {
    digitalWrite(PUMP_FOR, LOW);
    digitalWrite(PUMP_REV, LOW);
    delay(10);
    gDisplay.reinitDisplay();
}
