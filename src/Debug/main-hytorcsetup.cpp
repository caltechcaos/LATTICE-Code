#include <Arduino.h>

#include "HytorcSimple.h"
#include "Util.h"

lattice::HytorcSimple testMotor{7, 37, 38};
void setup() {
    lattice::GenericSetup();
    testMotor.Setup();
    testMotor.SetPercentOutput(0.0);
}

double power = 0.0;

void loop() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            power = 1;
        } else if (input == 'b') {
            power -= -.1;
        } else if (input == 'c') {
            power += .1;
        } else if (input == 'd') {
            power = -3;
        } else if (input == 'f') {
            power = 0;
        }
    }
    Serial.println(power);

    testMotor.SetVoltage(power, 18);
}