#include <Arduino.h>

#include "Util.h"
#include "Voltage.h"

lattice::Voltage ShuttleVoltage(A0);

void setup() {
    lattice::GenericSetup();
    ShuttleVoltage.Setup();
}

// the loop function runs over and over again forever
void loop() {
    Serial.println(ShuttleVoltage.CalculateFilteredVoltage());
}
