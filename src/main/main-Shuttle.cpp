#include <Arduino.h>
#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

#define RPMSCALE 5700

lattice::RC controller(Serial1, 19, 2, 13); // TODO: Use correct wiring when ATVR is fixed
lattice::Shuttle &shuttle = lattice::Shuttle::GetInstance();

// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
}

// Horizontal: Drive Left/Right
void updateShuttle(double x) {
    shuttle.SetMotionMotors(x * RPMSCALE);
}

void engageBreaks(double tensionBreak) {
    if (tensionBreak >= .75) {
        shuttle.EngageMotorBreak();
    } else if (tensionBreak <= .25) {
        shuttle.DisengageMotorBreak();
    }
}

void adjustTension(double tensionAdjust, int tensionReset, double tensionBreak) {
    switch (tensionReset) {
        case 0: // Manual Adjust
            shuttle.SetTensionArmPowers(tensionAdjust, tensionAdjust);
            break;
        case 1: // Reset
            shuttle.ResetTensionArms();
            break;
        case 2:
            engageBreaks(tensionBreak);
            break;
    }
}

enum auto_mode {
    StakeTransition,
    TakeupDrive
};

void loop() {
    controller.Update();
    double tensionBreak = -1 * controller.GetThrottle();
    double x = -1 * controller.GetAileron();
    double tensionAdjust = -1 * controller.GetElevator();
    int tensionReset = controller.GetGear();
    int ShuttleAutoDrive = controller.GetAux1();

    // Set stake number with Switch A, assume there's 3 for now
    auto_mode curr_mode = auto_mode::TakeupDrive;
    switch (ShuttleAutoDrive) {
        case 0: // Manual Mode
            updateShuttle(x);
            adjustTension(tensionAdjust, tensionReset, tensionBreak);
            break;
        case 1: // Autonomous Mode
            if (curr_mode == auto_mode::TakeupDrive) {
                if (shuttle.ConstantTakeupDrive()) {
                    curr_mode = auto_mode::StakeTransition;
                }
            } else {
                if (shuttle.StakeTransition()) {
                    curr_mode = auto_mode::TakeupDrive;
                }
            }
            break;
        case 2: // Emergency Stop
            shuttle.StopMotionMotors();
            break;
    }

    // Killswitch
    if (controller.GetAux2() == 1) {
        Serial.println("Killing Shuttle...");
        shuttle.EStop();
    };
    
}