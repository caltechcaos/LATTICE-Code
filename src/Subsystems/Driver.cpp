#include "Driver.h"

#include "Util.h"

using namespace lattice;

Driver::Driver() : elevator(kElevatorMotorPin, kElevatorEncoderFwdPin, kElevatorEncoderBckPin),
                   /* actuator(), */
                   handoff(kHandoffMotorPin1, kHandoffMotorPin2, kHandoffMotorPin3, kHandoffMotorPin4),
                   /* rc_input(), rc_output(), */
                   first_stake(kHandoffLimitSwitch1Pin),
                   second_stake(kHandoffLimitSwitch2Pin),
                   third_stake(kHandoffLimitSwitch3Pin),
                   elevator_zero(kElevatorTopLimitSwitchPin),
                   elevator_end(kElevatorBottomLimitSwitchPin),
                   actuator_temp(kDriverHytorcThermistorPin),
                   actuator_current(kDriverHytorcCurrentPin),
                   elevator_current(kElevatorCurrentPin) {}

void Driver::Setup() {
    elevator.Setup();

    first_stake.Setup();
    second_stake.Setup();
    third_stake.Setup();
    elevator_zero.Setup();
    elevator_end.Setup();
}

void Driver::UpdateSensors() {
    first_stake.Update();
    second_stake.Update();
    third_stake.Update();
    elevator_zero.Update();
    elevator_end.Update();
}

void Driver::EStop() {
    elevator.SetBrake(true);
    handoff.SetBrake(true);
}