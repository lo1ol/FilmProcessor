#include "Pump.h"

#include <Arduino.h>

#include "Config.h"

Pump::Pump() {
    pinMode(PUMP_IN1, OUTPUT);
    pinMode(PUMP_IN2, OUTPUT);
}

void Pump::loadInTank() {
    digitalWrite(PUMP_IN1, LOW);
    digitalWrite(PUMP_IN2, HIGH);
}

void Pump::loadOutTank() {
    digitalWrite(PUMP_IN1, HIGH);
    digitalWrite(PUMP_IN2, LOW);
}

void Pump::stop() {
    digitalWrite(PUMP_IN1, LOW);
    digitalWrite(PUMP_IN2, LOW);
}
