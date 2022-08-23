#pragma once
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "HandoffMotor.h"
#include "LimitSwitch.h"
#include "Thermistor.h"

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

    // RCInput rc_input;
    // Logger rc_output;

    LimitSwitch first_stake;
    LimitSwitch second_stake;
    LimitSwitch third_stake;

    LimitSwitch elevator_zero;  // top
    LimitSwitch elevator_end;   // bottom

    Thermistor actuator_temp;
    CurrentSensor actuator_current;
    CurrentSensor elevator_current;

    // PWM??
};
}  // namespace lattice