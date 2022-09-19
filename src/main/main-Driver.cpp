#include <Arduino.h>
#include <TaskScheduler.h>

#include "FeedforwardUtil.h"
#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Subsystems/Driver.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

// CONSTANTS
constexpr double RPMSCALE = 5700;

double kS = lattice::DriverConstants::kS;
double kV = lattice::DriverConstants::kV;
double kA = lattice::DriverConstants::kA;
constexpr double t_desired = 100;  // Nm
constexpr double f_desired = 100;  // Nm
constexpr double THETA = 1;
constexpr double I_s = 1;
constexpr double max_voltage = 100;

// sgn() function definiition
template <typename T>
int sgn(T num) {
    return (T(0) < num) - (num < T(0));
}

double DriverVoltage = lattice::GetSimpleFeedforward(kS, kV, kA, 0, t_desired / I_s);
double ElevatorVoltage = lattice::GetElevatorFeedforward(lattice::ElevatorConstants::kS, lattice::ElevatorConstants::kV, lattice::ElevatorConstants::kA, lattice::ElevatorConstants::kG, 0, f_desired);
constexpr double Battery = 100;  // TMP

// TODO: If limit switches are hit on elevator that we can’t move it anymore
// TODO: Autonomous torque and downforce application and Stake handoff
// - https://www.overleaf.com/read/kghkvywmkyfj
// - Use Latex file's math to calculate

lattice::RC controller(Serial3, lattice::RCPorts::kDriverRXPort, lattice::RCPorts::kDriverPowerPort);  // TODO: Use correct wiring when ATVR is fixed
lattice::Clifford &clifford = lattice::Clifford::clifford();
lattice::Driver &driver = lattice::Driver::driver();
lattice::HytorcSimple hytorcSimple{9, 5, 6};  // TODO: Temp Ports

// r: forward/backward translation
// theta: rotation
bool updateClifford(double r, double theta) {
    return clifford.Move(r, theta);
}

// Vertical: Move Elevator
// Horizontal: Switch Stake
bool updateElevator(double y, double drill) {
    driver.SetElevatorPower(y);
    hytorcSimple.SetPercentOutput(drill);
    return true;
}

bool autoDrill() {
    driver.SetDriverVoltage(DriverVoltage);
    driver.SetElevatorVoltage(ElevatorVoltage);
    return true;
}

// Horizontal: Drive Left/Right
// bool updateShuttle(double x) {
//     shuttle.SetMotionMotors(x * RPMSCALE);
//     return true;
// }

int stakeIterations = 0;

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
            if (y_left >= 0.9) {
                autoDrill();
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