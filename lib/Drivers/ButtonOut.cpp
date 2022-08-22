#include "ButtonOut.h"
#include <Arduino.h>

using namespace lattice;

ButtonOut::ButtonOut(const int PIN) : M_PIN(PIN) { }

void ButtonOut::Setup() {
    pinMode(M_PIN, OUTPUT);
    Set(false);
}

void ButtonOut::Set(bool value) {
    digitalWrite(M_PIN, value ? HIGH : LOW);
}