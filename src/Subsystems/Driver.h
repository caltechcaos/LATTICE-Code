#pragma once
#include "CurrentSensor.h"
#include "HandoffMotor.h"
#include "LimitSwitch.h"
#include "Thermistor.h"

namespace lattice {
class Driver {
   public:
    Driver();

   private:
    // ElevatorMotor elevator;
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
    CurrentSensor driver_current;
};
}  // namespace lattice