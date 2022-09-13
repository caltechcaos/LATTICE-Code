// TODO:  Manual Mode (Control arms and inner/outer motors separately)
// -  Make sure that the arms can’t be controlled at the same time as motion motors
// TODO: Autonomous Mode
// - Automatic traversal of cable + stake transition
//   - Need to somehow indicate which side is the “front side”
// - Although can indicate when to stop (not estop)

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
bool updateShuttle(double x) {
    shuttle.SetMotionMotors(x * RPMSCALE);
    return true;
}


enum auto_mode {
    StakeTransition,
    TakeupDrive,
};

void loop() {
    controller.Update();

    double x = -1 * controller.GetAileron();
    int ShuttleAutoDrive = controller.GetAux1();

    // Set stake number with Switch A, assume there's 3 for now
    auto_mode curr_mode = auto_mode::TakeupDrive;
    switch (ShuttleAutoDrive) {
        case 0: // Manual Mode
            if (!updateShuttle(x))
                Serial.println("Failure in Shuttle updateShuttle()");
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
        shuttle.EStop();
        Serial.println("Aux 2: Killed shuttle");
    };
    
}