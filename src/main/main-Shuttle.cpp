#include <Arduino.h>
#include <TaskScheduler.h>

#include <cstdlib>

#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

#define RPMSCALE 5700

lattice::RC controller(Serial1, 19, 2, 13);  // TODO: Use correct wiring when ATVR is fixed
lattice::Shuttle &shuttle = lattice::Shuttle::GetInstance();

// Horizontal: Drive Left/Right
void updateShuttle(double x) {
    shuttle.SetMotion(x * RPMSCALE);
}

void engageBreaks(double tensionBreak) {
    if (tensionBreak >= .75) {
        shuttle.EngageMotorBreak();  // not yet implemented
    } else if (tensionBreak <= .25) {
        shuttle.DisengageMotorBreak();  // not yet implemented
    }
}

void adjustTension(double tensionAdjust, int tensionReset, double tensionBreak) {
    switch (tensionReset) {
        case 0:  // Manual Adjust
            shuttle.SetTensionArmPowers(tensionAdjust, tensionAdjust);
            break;
        case 1:  // Reset
            shuttle.ResetTensionArms();
            break;
        case 2:
            engageBreaks(tensionBreak);
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
            if (abs(y_left) >= 0.9 && curr_mode == mode::TakeupDrive) {
                shuttle.ConstantTakeupDrive();
                curr_mode = mode::StakeTransition;
                // Down - Switch from autonomous stake transition to autonomous constant takeup
            } else if (abs(y_left) <= 0.1 && curr_mode == mode::StakeTransition) {
                shuttle.StakeTransition();
                curr_mode = mode::TakeupDrive;
            }
            break;
        case 2:  // Emergency Stop
            shuttle.StopMotionMotors();
            break;
    }

    // Killswitch
    if (controller.GetAux2() == 1) {
        Serial.println("Killing Shuttle...");
        shuttle.EStop();
    };
}

Scheduler ts;
constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &shuttleLoop);

// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
    shuttle.Setup();

    Serial.begin(115200);

    ts.init();
    ts.addTask(mainLoop);
    delay(5000);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}