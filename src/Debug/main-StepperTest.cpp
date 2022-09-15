#include <Arduino.h>
#include <AccelStepper.h>
#include "Util.h"
#include "StepperHandOff.h"
//#include "HandoffMotor.h"

using namespace lattice;
//using namespace lattice::HandoffConstants;

//lattice::HandoffMotor StepperDriver(kDrivePin, kStepPin, kStepsPerRev);//, kRPM, kMicrosteps

int stepsPerRevolution = 200;
//AccelStepper stepper = AccelStepper(MotorInterfaceType, 8, 9, 10, 11);
//AccelStepper myStepper(4, 8, 9, 10, 11);
lattice::StepperHandOff Handoff(4, HandoffConstants::kStepPin1, HandoffConstants::kStepPin2, HandoffConstants::kStepPin3, HandoffConstants::kStepPin4);
int Speed; 

void setup() {
    lattice::GenericSetup();
    //myStepper.setMaxSpeed(1000);
    Handoff.Setup(1000);
    //StepperDriver.Setup();
    //StepperDriver.SetAccel(kMotorAccel, kMotorDecel);
    //myStepper.setSpeed(60);
}

void loop() {
    if(Serial.available()){
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            Speed = 50;
        } else if (input == 'b') {
            Speed = -50;
        } else if (input == 'c') {
            Speed = 100;
        } else if (input == 'd') {
            Speed = -100;
        } else if (input == 'f') {
            Speed = 0;
        }

    }
    Handoff.SetSpeed(Speed);
    //StepperDriver.StepMove(steps);    
    //Handoff.run();
    Serial.println(Speed);

}
