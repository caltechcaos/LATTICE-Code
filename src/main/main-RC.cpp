#include <Arduino.h>
#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Subsystems/Driver.h"
#include "Util.h"

lattice::RC controller(Serial1, 19, 2, 13);
auto& clifford = lattice::Clifford::clifford();
auto& driver = lattice::Driver::driver();

// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
    driver.Setup();
}

// RUns continously
void loop() {
    controller.Update();

    // Clifford Translational Motion
    double x = controller.GetThrottle();
    double y = controller.GetElevator();
    clifford.Move(x, y);

    // Driver 
    double x_Driver = controller.GetAileron(); // hand off motor
    double y_Driver = controller.GetRudder(); // elevator motor
    
    
    // Switch
    switch (controller.GetAux2()) {
        case 0:
            driver.ZeroElevator();
            break;
        default:
            driver.SetStake(lattice::Driver::StakeNumber::kOne);
    }
}


// TODO: Elevator controls
// TODO: Switch (Planting, Drilling)