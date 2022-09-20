#include "HallEffect.h"

#include <Arduino.h>
using namespace lattice;

HallEffect::HallEffect(int signalPort) : kPort(signalPort) {}

void HallEffect::Setup() {
    pinMode(kPort, INPUT);
    digitalWrite(kPort, LOW);
}
bool HallEffect::Get() {
    return !digitalRead(kPort);
}