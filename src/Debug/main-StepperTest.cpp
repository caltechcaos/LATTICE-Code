#include <Arduino.h>
#include <Stepper.h>
#include "Util.h"
//#include "HandoffMotor.h"

using namespace lattice;
using namespace lattice::HandoffConstants;

//lattice::HandoffMotor StepperDriver(kDrivePin, kStepPin, kStepsPerRev);//, kRPM, kMicrosteps

int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
int steps; 

void setup() {
    lattice::GenericSetup();
    //StepperDriver.Setup();
    //StepperDriver.SetAccel(kMotorAccel, kMotorDecel);
    myStepper.setSpeed(60);
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
    //StepperDriver.StepMove(steps);    
    myStepper.step(steps);
    Serial.println(steps);

}
