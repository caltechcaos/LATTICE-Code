#include "Driver.h"

#include <tuple>

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
                         GetSimpleFeedforward(kSDriver, kVDriver, kADriver, 0, 0)),
      state(State::Idle) {}

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

void Driver::Run() {
    UpdateSensors();
    // TODO: state machine stuff
}

void Driver::EStop() {
    elevator.SetBrake(true);
    // actuator.SetBrake(true);
    handoff.SetBrake(true);
}

bool Driver::RunElevatorOneTick(double setpoint) {
    double feedback = elevatorCurrent.Get();
    double input;
    bool success;
    std::tie(input, success) = actuatorController.Run(feedback, setpoint);

    if (success) {
        if (input < kElevatorMinVoltage) {  // voltage drop
            // TODO: throw a warning that we might be stuck
        }
        if (!elevator.Run(input)) {
            // TODO: throw a warning for elevator input out of bounds
        }

        delay(kElevatorLoopDelay);
        if (elevatorEnd.Pushed()) {
            return true;
        }
        // TODO: calculate velocity, if it's zero, throw a warning that we might be stuck
    } else {
        // TODO: throw a warning for elevator controller not evaluating?
    }

    return false;
}

bool Driver::ZeroElevator() {
    if (state == State::ZeroElevator) {
        if (elevatorZero.Get()) {
            elevator.Run(0);  // stop
            state = State::Idle;
            return true;
        }
    } else if (state == State::Idle) {
        if (elevatorZero.Get()) {
            // TODO: throw warning of trying to zero elevator when already zeroed
            return true;
        }
        state = State::ZeroElevator;
        elevator.Run(-kElevatorZeroSpeed);
    } else {
        // TODO: throw warning that we can't zero the elevator right now
        // add way to override it?
    }
    return false;
}