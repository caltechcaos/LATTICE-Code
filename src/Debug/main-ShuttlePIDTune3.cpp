#include <TaskScheduler.h>

#include "HytorcSimple.h"
#include "PIDF.h"
#include "Util.h"

constexpr double kP = 1;
constexpr double kI = 0.0;
constexpr double kD = 0.5;
constexpr double kVolts = 18.0;
double kSetpoint = 36.5;
constexpr double kS = 1.7;

lattice::HytorcSimple mMotor{32, 24, 25};
lattice::PIDF mController{kP, kI, kD, [](double s) { return copysign(kS, s); }, 0.5, 1};

constexpr int period = 20;
Scheduler ts;

double power = 0.0;
bool pidMode = false;
bool counterStall = false;

double bound(double input, double minimum, double target, double pos) {
    double error = target - pos;
    if (((input >= 0) && (error >= 0)) || ((input < 0) && (error < 0))) {
        if (abs(input) < abs(minimum)) {
            return copysign(minimum, error);
        } else {
            return input;
        }

    } else {
        return copysign(minimum, error);
    }
}
void run() {
    if (pidMode) {
        power = bound(mController.Run(-mMotor.GetPosition() * 360.0 / 5000.0), kS, kSetpoint, -mMotor.GetPosition() * 360.0 / 5000.0);
        if (mController.AtTarget()) {
            power = 0.0;
        }
        // Serial.println(mController.AtTarget());
        //  Serial.println("RAN PID");
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
            kSetpoint = 25.5;
            mController.SetTarget(kSetpoint);
            power = 0.0;
            pidMode = true;
        } else if (input == 'o') {
            kSetpoint = 36.5;
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
    Serial.println(-mMotor.GetPosition() * 360.0 / 5000.0);
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