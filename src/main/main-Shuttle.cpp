#include <Arduino.h>
#include <TaskScheduler.h>

#include <cstdlib>

#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

constexpr double kRPMScale = 5700;
constexpr double kTakeup = 0.25;  // Required takeup in meters.

lattice::RC controller(Serial1, 19, 2);  // TODO: Use correct wiring when ATVR is fixed
lattice::Shuttle &shuttle = lattice::Shuttle::GetInstance();

// Horizontal: Drive Left/Right
void updateShuttle(double x) {
    shuttle.SetMotion(x * kRPMScale);
}

void engageBreaks(double tensionBreak) {
    if (tensionBreak >= .75) {
        shuttle.EngageMotorBreak();  // not yet implemented
    } else if (tensionBreak <= -.75) {
        shuttle.DisengageMotorBreak();  // not yet implemented
    }
}

void adjustTension(double tensionAdjust, int tensionReset, double tensionBreak) {
    switch (tensionReset) {
        case 0:  // Manual Adjust
            shuttle.SetTensionArmPowers(tensionAdjust, tensionAdjust);
            break;
        case 1:  // Reset
            engageBreaks(tensionBreak);
            break;
        case 2:
            shuttle.ResetTensionArms();
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

    double y_left = -1 * controller.GetThrottle();   // tensionBreak
    double y_right = -1 * controller.GetElevator();  // tensionAdjust
    double x_right = -1 * controller.GetAileron();   // x

    int gearPosition = controller.GetGear();  // tensionReset
    int aux1 = controller.GetAux1();          // shuttleAutoDrive

    // Control front of shuttle, assume default gear position is 0
    if (controller.GetGear() == 0) {
        shuttle.SetFrontLimitSwitch(lattice::Shuttle::FrontLimitSwitch::kLeft);
    } else if (controller.GetGear() == 2) {
        shuttle.SetFrontLimitSwitch(lattice::Shuttle::FrontLimitSwitch::kRight);
    }

    // Set stake number with Switch A, assume there's 3 for now
    mode curr_mode = mode::TakeupDrive;
    switch (aux1) {
        case 0:  // Manual Mode
            updateShuttle(x_right);
            adjustTension(y_right, gearPosition, y_left);
            break;
        case 1:  // Transition Mode
            // Up - Switch from autonomous constant takeup to autonomous stake transition
            if (curr_mode == mode::TakeupDrive) {
                if (shuttle.ConstantTakeupDrive(abs(y_left) >= 0.9)) {
                    curr_mode = mode::StakeTransition;
                }
            } else {
                if (shuttle.StakeTransition(abs(y_left) <= 0.1)) {
                    curr_mode = mode::TakeupDrive;
                }
            }
            break;
        case 2:  // Emergency Stop
            shuttle.StopMotionMotors();
            break;
    }

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