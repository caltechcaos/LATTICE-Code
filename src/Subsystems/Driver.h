#pragma once
#include "Clifford.h"
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "HandoffMotor.h"
#include "LimitSwitch.h"
#include "PIDF.h"
#include "Thermistor.h"

namespace lattice {
class Driver {
   public:
    /**
     * Singleton to ensure we only have one driver subsystem floating around
     * @returns The single instance of the driver
     */
    static Driver& driver() {
        static Driver driver;
        return driver;
    }

    /**
     * Sets up to use the driver subsystem. Must be called before
     * using it.
     */
    void Setup();

    /**
     * Updates all sensors which require an update. Must be called
     * each loop cycle, before using the driver.
     */
    void UpdateSensors();

    /**
     * Flips the emergency stop and freezes all actuators
     */
    void EStop();

    // Guarantee the singleton
    Driver(Driver const&) = delete;
    void operator=(Driver const&) = delete;

   private:
    ElevatorMotor elevator;
    // HytorcMotor actuator;
    HandoffMotor handoff;

    // RCInput rcInput;
    // Logger rcOutput;

    LimitSwitch firstStake;
    LimitSwitch secondStake;
    LimitSwitch thirdStake;

    LimitSwitch elevatorZero;  // top
    LimitSwitch elevatorEnd;   // bottom

    Thermistor actuatorTemp;
    CurrentSensor actuatorCurrent;
    CurrentSensor elevatorCurrent;

    PIDF elevatorController;
    PIDF actuatorController;

    /**
     * Creates the driver subsystem, using the pins in Util.h
     */
    Driver();

    /**
     * Runs the elevator controller one tick (using PIDF current control)
     * @param setpoint The target current, in amps
     */
    void RunElevatorOneTick(double setpoint);
};

}  // namespace lattice