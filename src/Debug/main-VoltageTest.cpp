#include <Arduino.h>
#include "Voltage.h"
#include "Util.h"

lattice::Voltage ShuttleVoltage(lattice::ShuttleConstants::kVoltagePin);

// float counts = 0;
// float mv = 0;

// const float mvc = 3.69; input voltage divided by 1024


void setup() {
    lattice::GenericSetup();
    ShuttleVoltage.Setup();
}

// the loop function runs over and over again forever
void loop() {
    //Serial.println(analogRead(lattice::ShuttleConstants::kVoltagePin));
    Serial.print("volatge at : ");
    Serial.println(ShuttleVoltage.CalculateVoltage(lattice::ShuttleConstants::kMaxInputVoltage,
     lattice::ShuttleConstants::kMultiplier));
    // counts = analogRead(lattice::ShuttleConstants::kVoltagePin);
    // Serial.println(String(analogRead(counts)));

    // mv
}
