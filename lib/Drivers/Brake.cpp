#include "Brake.h"
#include <Arduino.h>

using namespace lattice;

Brake::Brake(const int PIN) : M_PIN(PIN) { }

void Brake::Setup() {
    pinMode(M_PIN, OUTPUT);
    Set(false);
}
void Brake::Set(bool value) {
    digitalWrite(M_PIN, value ? HIGH : LOW);
}
