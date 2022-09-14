#include <Arduino.h>

#include "Util.h"
#include "HandoffMotor.h"

using namespace lattice;
using namespace lattice::HandoffConstants;

lattice::HandoffMotor StepperDriver(kDrivePin, kStepPin, kStepsPerRev, kRPM, kMicrosteps);

int steps; 

void setup() {
    lattice::GenericSetup();
    StepperDriver.Setup();
    StepperDriver.SetAccel(kMotorAccel, kMotorDecel);
}

void loop() {
    if(Serial.available()){
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            steps = 700;
        } else if (input == 'b') {
            steps = -700;
        } else if (input == 'c') {
            steps = 1400;
        } else if (input == 'd') {
            steps = -1400;
        } else if (input == 'f') {
            steps = 0;
        }

    }
    StepperDriver.StepMove(steps);

}
