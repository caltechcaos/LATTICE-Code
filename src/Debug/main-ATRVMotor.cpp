#include <Arduino.h>

#include "ATRVMotor.h"
#include "Util.h"
lattice::ATRVMotor motor{12, 13};

void setup() {
    lattice::GenericSetup();
    motor.Setup();
}
double power = 0;
bool enable = false;
void loop() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            power = 0.5;
        } else if (input == 'b') {
            power = -0.5;
        } else if (input == 'c') {
            power = 0;
        }
    }
    motor.SetPower(power);
    Serial.println(power);
}