#include "EStop.h"

#include <Arduino.h>

using namespace lattice;

EStop::EStop(const int PIN) : M_PIN(PIN) {}

void EStop::Setup() {
    pinMode(M_PIN, OUTPUT);

    Set(false);
}
void EStop::Set(bool value) {
    auto pinVal = value ? HIGH : LOW;
    digitalWrite(M_PIN, pinVal);
}

void EStop::Disable() {
    Serial.println("STOPPED");
    Set(false);
}

void EStop::Enable() {
    Set(true);
}
