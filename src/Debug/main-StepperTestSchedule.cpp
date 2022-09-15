#include <AccelStepper.h>
#include <Arduino.h>
// #include <Stepper.h>
//#include "DRV8834.h"
#include "TaskScheduler.h"
#include "Util.h"
//#include "HandoffMotor.h"

using namespace lattice;
// using namespace lattice::HandoffConstants;

// lattice::HandoffMotor StepperDriver(kDrivePin, kStepPin, kStepsPerRev);//, kRPM, kMicrosteps

int stepsPerRevolution = 200;
// AccelStepper stepper = AccelStepper(MotorInterfaceType, 8, 9, 10, 11);
AccelStepper myStepper(4, 8, 9, 10, 11);

// lattice::StepperHandOff Handoff(4, HandoffConstants::kStepPin1, HandoffConstants::kStepPin2, HandoffConstants::kStepPin3, HandoffConstants::kStepPin4);
int Speed;
Scheduler ts;
void run() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            Speed = 600;
        } else if (input == 'b') {
            Speed = -600;
        } else if (input == 'c') {
            Speed = 100;
        } else if (input == 'd') {
            Speed = -100;
        } else if (input == 'f') {
            Speed = 0;
        }
    }
    myStepper.setSpeed(Speed);
    myStepper.runSpeed();

    Serial.println(Speed);
}
constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &run);

void setup() {
    lattice::GenericSetup();
    // stepper.begin(120, 1);
    myStepper.setMaxSpeed(1000);
    //  Handoff.Setup(1000);
    //  StepperDriver.Setup();
    //  StepperDriver.SetAccel(kMotorAccel, kMotorDecel);
    // myStepper.setSpeed(60);
    ts.init();
    Serial.println("Initialized scheduler");

    ts.addTask(mainLoop);
    Serial.println("added main task");
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}
