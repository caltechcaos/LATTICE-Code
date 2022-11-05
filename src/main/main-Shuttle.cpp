#include <Arduino.h>
#include <TaskScheduler.h>

#include <cstdlib>

#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

constexpr double kRPMScale = 5700;
constexpr double kTakeup = 0.25;  // Required takeup in meters.
constexpr double kMaxVoltage = 9;
lattice::RC controller(Serial3, lattice::RCPorts::kShuttleRXPort, lattice::RCPorts::kShuttlePowerPort);
lattice::Shuttle &shuttle = lattice::Shuttle::GetInstance();

// Horizontal: Drive Left/Right
void updateShuttle(double hold, double regular, int gearPosition) {
    switch (gearPosition) {
        case 0:  // Manual Adjust
            shuttle.SetMotion(hold * kRPMScale);
            break;
        case 1:  // Reset
            shuttle.SetMotion(regular * kRPMScale);
            break;
        case 2:
            shuttle.SetMotion(0.0);
            break;
    }
}

void engageBreaks(double tensionBreak) {
    if (tensionBreak >= .75) {
        shuttle.EngageMotorBreak();  // not yet implemented
    } else if (tensionBreak <= -.75) {
        shuttle.DisengageMotorBreak();  // not yet implemented
    }
}

void adjustTension(double left, int tensionReset, double right) {
    switch (tensionReset) {
        case 0:  // Manual Adjust
            shuttle.DisengageMotorBreak();
            shuttle.SetTensionArmPowers(left, right);
            break;
        case 1:  // Reset
            shuttle.EngageMotorBreak();
            shuttle.SetTensionArmPowers(0.0, 0.0);
            break;
        case 2:
            shuttle.SetTensionArmPowers(0.0, 0.0);
            break;
    }
}

enum mode {
    StakeTransition,
    TakeupDrive
};

void shuttleLoop() {
    controller.Update();
    shuttle.UpdateSensors();
    // Killswitch
    if (controller.GetAux2() == 1) {
        Serial.println("Killing Shuttle...");
        shuttle.EStop();
        return;
    };

    double y_left = controller.GetThrottle();   // tensionBreak
    double y_right = controller.GetElevator();  // tensionAdjust
    double x_right = -controller.GetAileron();  // x
    double x_left = controller.GetRudder();     // x

    int gearPosition = controller.GetGear();  // tensionReset
    int aux1 = controller.GetAux1();          // shuttleAutoDrive

    // Control front of shuttle, assume default gear position is 0
    if (controller.GetGear() == 0) {
        shuttle.SetFrontLimitSwitch(lattice::Shuttle::FrontLimitSwitch::kLeft);
    } else if (controller.GetGear() == 2) {
        shuttle.SetFrontLimitSwitch(lattice::Shuttle::FrontLimitSwitch::kRight);
    }

    mode curr_mode = mode::TakeupDrive;
    switch (aux1) {
        case 0:  // Manual Mode
            shuttle.StopMotionMotors();
            shuttle.SetTensionArmPowers(0, 0);
            break;

        case 1:  // Transition Mode
            Serial.println("Motion");
            updateShuttle(y_left, y_right, gearPosition);
            break;
        case 2:  // Stop moving
            Serial.println("ARMS");
            adjustTension(x_left * kMaxVoltage, gearPosition, x_right * kMaxVoltage);
            break;
    }
    Serial.print(shuttle.GetBatteryVoltage());
    Serial.print(", ");
    Serial.print(shuttle.GetLeftTensionArmPos());
    Serial.print(", ");
    Serial.println(shuttle.GetRightTensionArmPos());
}

Scheduler ts;
constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &shuttleLoop);

// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
    shuttle.Setup();
    shuttle.SetTakeup(kTakeup);

    ts.init();
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}