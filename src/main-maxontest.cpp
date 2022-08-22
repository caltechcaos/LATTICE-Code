#include <Arduino.h>
#include "MotionMotor.h"
#include "Util.h"

lattice::MotionMotor testMotor{50, 2, 0};
double RPMCommand = 0;
int direction = 1;
void setup() {
    lattice::GenericSetup();
    testMotor.Setup();
}

void loop() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            testMotor.EnableMotor();
        }
        if (input == 'b') {
            testMotor.DisableMotor();
        }
        if (input == 'c') {
            RPMCommand = 200;
        }
        if (input == 'd') {
            RPMCommand = 3000;
        }
        if (input == 'e') {
            RPMCommand = 6000;
        }
        if (input == 'f') {
            RPMCommand = 0;
        }
        if (input == 'g') {
            direction *= -1;
        }
    }
    testMotor.SetVelocity(direction * RPMCommand);
    Serial.println(testMotor.GetVelocity());
}