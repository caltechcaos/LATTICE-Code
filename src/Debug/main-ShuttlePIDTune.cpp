#include <TaskScheduler.h>

#include "HytorcSimple.h"
#include "PIDF.h"
#include "Util.h"

constexpr double kP = 0.0725;
constexpr double kI = 0.0;
constexpr double kD = 0.0;
constexpr double kVolts = 18.0;
double kSetpoint = -200;
constexpr double kS = 1.2;

lattice::HytorcSimple mMotor{9, 5, 6};
lattice::PIDF mController{kP, kI, kD, [](double s) { return copysign(kS, s); }, 4, 0.01};

constexpr int period = 20;
Scheduler ts;

double power = 0.0;
bool pidMode = false;
bool counterStall = false;
void run() {
    if (pidMode) {
        power = mController.Run(mMotor.GetPosition());
        if (mController.AtTarget()) {
            power = 0.0;
        }
        // Serial.println("RAN PID");
    }
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();
        if (input == 'a') {
            pidMode = false;
            power = 0.25 * kVolts;
        } else if (input == 'b') {
            pidMode = false;
            power = -0.25 * kVolts;
        } else if (input == 'r') {
            mMotor.ResetEncoderPosition();
        } else if (input == 's') {
            kSetpoint = -500;
            mController.SetTarget(kSetpoint);
            power = 0.0;
            pidMode = true;
        } else if (input == 'o') {
            kSetpoint = 500;
            mController.SetTarget(kSetpoint);
            power = 0.0;
            pidMode = true;
        } else if (input == 'p') {
            pidMode = false;
            power = -1 * kVolts;
        } else if (input == 'f') {
            pidMode = false;
            power = 0;
        }
    }

    mMotor.SetVoltage(power, kVolts);
    Serial.print(power);
    Serial.print(", ");
    Serial.println(mMotor.GetPosition());
}
Task mainLoop(period, TASK_FOREVER, &run);

void setup() {
    lattice::GenericSetup();
    mMotor.Setup();
    mMotor.SetPercentOutput(0.0);
    mController.SetTarget(kSetpoint);
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}