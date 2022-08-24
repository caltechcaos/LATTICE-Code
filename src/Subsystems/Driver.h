#pragma once
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "HandoffMotor.h"
#include "LimitSwitch.h"
#include "Thermistor.h"
#include "PIDF.h"

namespace lattice {
class Driver {
   public:
    /**
     * Creates the driver subsystem, using the pins in Util.h
     */
    Driver();

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
     * Runs the elevator controller one tick (using PIDF current control)
     * @param setpoint The target current, in amps
     */
    void RunElevatorController(double setpoint);
};
}  // namespace lattice