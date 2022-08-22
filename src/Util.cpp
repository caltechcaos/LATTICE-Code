#include "Util.h"
#include "Constants.h"
#include <Arduino.h>

using namespace lattice;

void lattice::GenericSetup() {
    analogWriteResolution(ANALOG_BITS);
    analogReadResolution(ANALOG_BITS);
    Serial.begin(SERIAL_RATE);
}