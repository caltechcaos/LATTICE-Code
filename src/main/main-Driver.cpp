#include <Arduino.h>
#include <TaskScheduler.h>

#include "FeedforwardUtil.h"
#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Subsystems/Driver.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

lattice::RC controller(Serial3, lattice::RCPorts::kDriverRXPort, lattice::RCPorts::kDriverPowerPort);
lattice::Clifford &clifford = lattice::Clifford::clifford();
lattice::Driver &driver = lattice::Driver::driver();

// r: forward/backward translation
// theta: rotation
bool updateClifford(double r, double theta) {
    return clifford.Move(r, theta);
}

// Vertical: Move Elevator
// Horizontal: Move drill
bool updateElevator(double y, double drill) {
    driver.SetElevatorPower(y);
    driver.SetDriverPower(drill);
    return true;
}
int prevAux = 0;
bool stakeHandoff() {
    driver.RunStakeHandoff();
    return true;
}

// Runs continously
void driverLoop() {
    controller.Update();
    driver.UpdateSensors();

    // Killswitch
    if (controller.GetAux2() == 1) {
        driver.EStop();
        clifford.SetBrake(true);
        clifford.Move(0, 0);
        Serial.println("Aux 2: Killed shuttle");
        return;
    };

    // Translational Motion of Driver
    double x_left = controller.GetRudder();  // [0, 1] -> [-1, 1]
    double x_right = controller.GetAileron();

    double y_left = controller.GetThrottle();
    double y_right = controller.GetElevator();

    // Elevator
    int aux1 = controller.GetAux1();

    // Set stake number with Switch A, assume there's 3 for now
    switch (aux1) {
        case 0:
            driver.SetStake(lattice::Driver::StakeNumber::kOne);
            break;
        case 1:
            driver.SetStake(lattice::Driver::StakeNumber::kTwo);
            break;
        case 2:
            driver.SetStake(lattice::Driver::StakeNumber::kThree);
            break;
    }
    if (aux1 != prevAux) {
        driver.InitializeStakeHandoff();
    }
    prevAux = aux1;

    // F Switch Finite State Machine
    bool success;
    bool initializedStakeHandoff = false;
    switch (controller.GetGear()) {
        case 0:  // Move clifford
            success = updateClifford(y_right, -x_right);
            updateElevator(0, 0);
            break;
        case 1:  // Update elevator
            if (y_left >= 0.7) {
                driver.DriveStake();
            } else {
                success = updateElevator(y_right, x_left);
            }

            updateClifford(0, 0);
            break;
        case 2:  // Stake Handoff
            if (y_right >= 0.9) {
                success = stakeHandoff();
            }
            updateElevator(0, 0);
            updateClifford(0, 0);
            break;
        default:
            success = false;
            Serial.println("Unexpected return val for controller.GetGear()");
    };
    Serial.println(driver.GetBatteryVoltage());

    if (!success) {
        Serial.println("Failure in Finite State Machine for RC");
    }
}

Scheduler ts;
constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &driverLoop);

// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
    driver.Setup();
    clifford.Setup();

    Serial.begin(115200);

    ts.init();
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
    driver.InitializeStakeHandoff();
}

void loop() {
    ts.execute();
}