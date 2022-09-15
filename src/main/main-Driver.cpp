#include <Arduino.h>
#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/Driver.h"
#include "Util.h"
#include "FeedforwardUtil.h"
#include <TaskScheduler.h>

// CONSTANTS
constexpr double RPMSCALE = 5700;

double kS = lattice::DriverConstants::kS;
double kV = lattice::DriverConstants::kV;
double kA = lattice::DriverConstants::kA;
constexpr double t_desired = 100; // Nm
constexpr double THETA = 1;
constexpr double I_s = 1;
constexpr double max_voltage = 100;

// sgn() function definiition
template <typename T> int sgn(T num) {
    return (T(0) < num) - (num < T(0));
}

double DesiredVoltage = lattice::GetSimpleFeedforward(kS, kV, kA, 0, t_desired/I_s);
constexpr double Battery = 100; // TMP

// TODO: If limit switches are hit on elevator that we can’t move it anymore
// TODO: Autonomous torque and downforce application and Stake handoff
// - https://www.overleaf.com/read/kghkvywmkyfj
// - Use Latex file's math to calculate

lattice::RC controller(Serial1, 19, 2, 13); // TODO: Use correct wiring when ATVR is fixed
lattice::Clifford &clifford = lattice::Clifford::clifford();
lattice::Driver &driver = lattice::Driver::driver();
lattice::HytorcSimple hytorcSimple{9, 5, 6}; // TODO: Temp Ports



// Vertical: Move Drive Train Up/Down
// Horizontal: Move Drive Train Left/Right
bool updateClifford(double x, double y) {
    // clifford.Move(x, y); // TODO: Implement clifford.Move()
    return true;
}

// Vertical: Move Elevator
// Horizontal: Switch Stake
bool updateElevator(double y, double drill) {
    driver.SetElevatorPower(y);
    hytorcSimple.SetPercentOutput(drill);
    return true;
}

bool autoDrill() {
    hytorcSimple.SetVoltage(DesiredVoltage, Battery);
    return true;
}

// Horizontal: Drive Left/Right
// bool updateShuttle(double x) {
//     shuttle.SetMotionMotors(x * RPMSCALE);
//     return true;
// }

// Runs continously
void driverLoop() {
    controller.Update();

    // Killswitch
    if (controller.GetAux2() == 1) {
        driver.EStop();
        Serial.println("Aux 2: Killed shuttle");
    };

    
    // Translational Motion of Driver
    double x_right = -1 * controller.GetAileron();
    double y_right = controller.GetElevator();
    // Elevator 
    double x_left = (controller.GetRudder() - 0.5) * -2;
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

    // F Switch Finite State Machine
    bool success;
    switch (controller.GetGear()) {
        case 0:
            success = updateClifford(x_right, y_right);
            break;
        case 1:
            success = updateElevator(y_right, x_left);
            break;
        case 2:
        //     success = updateShuttle(x);
            break;
        default:
            success = false;
            Serial.println("Unexpected return val for controller.GetGear()");
    };

    if (!success) {
        Serial.println("Failure in Finite State Machine for RC");
    }
   
}

Scheduler ts;
constexpr int period = 5;

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
    delay(5000);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}