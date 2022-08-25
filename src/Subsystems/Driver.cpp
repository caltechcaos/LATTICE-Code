#include "Driver.h"

#include "FeedforwardUtil.h"
#include "Util.h"

using namespace lattice;

Driver::Driver()
    : elevator(kElevatorMotorPin, kElevatorEncoderFwdPin, kElevatorEncoderBckPin),
      /* actuator(), */
      handoff(kHandoffMotorPin1, kHandoffMotorPin2, kHandoffMotorPin3, kHandoffMotorPin4),
      /* rc_input(), rc_output(), */
      firstStake(kHandoffLimitSwitch1Pin),
      secondStake(kHandoffLimitSwitch2Pin),
      thirdStake(kHandoffLimitSwitch3Pin),
      elevatorZero(kElevatorTopLimitSwitchPin),
      elevatorEnd(kElevatorBottomLimitSwitchPin),
      actuatorTemp(kDriverHytorcThermistorPin),
      actuatorCurrent(kDriverHytorcCurrentPin),
      elevatorCurrent(kElevatorCurrentPin),
      elevatorController(kPElevator, kIElevator, kDElevator,
                         GetElevatorFeedforward(kSElevator, kVElevator, kAElevator, kGElevator, 0, 0)),
      actuatorController(kPDriver, kIDriver, kDDriver,
                         GetSimpleFeedforward(kSDriver, kVDriver, kADriver, 0, 0))
      {}

void Driver::Setup() {
    elevator.Setup();

    firstStake.Setup();
    secondStake.Setup();
    thirdStake.Setup();
    elevatorZero.Setup();
    elevatorEnd.Setup();
}

void Driver::UpdateSensors() {
    firstStake.Update();
    secondStake.Update();
    thirdStake.Update();
    elevatorZero.Update();
    elevatorEnd.Update();
}

void Driver::EStop() {
    elevator.SetBrake(true);
    // actuator.SetBrake(true);
    handoff.SetBrake(true);
}

void Driver::RunElevatorOneTick(double setpoint) {
    
}