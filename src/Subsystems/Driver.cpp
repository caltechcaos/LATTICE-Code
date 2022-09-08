#include "Driver.h"

#include <tuple>

#include "FeedforwardUtil.h"
#include "Util.h"

using namespace lattice;

Driver::Driver()
    : elevator(ElevatorConstants::kMotorPin, ElevatorConstants::kEncoderFwdPin, ElevatorConstants::kEncoderBckPin),
      /* actuator(), */
      handoff(HandoffConstants::kMotorPin1, HandoffConstants::kMotorPin2, HandoffConstants::kMotorPin3, HandoffConstants::kMotorPin4),
      /* rc_input() */
      logger(Logger::logger()),
      firstStake(HandoffConstants::kLimitSwitch1Pin),
      secondStake(HandoffConstants::kLimitSwitch2Pin),
      thirdStake(HandoffConstants::kLimitSwitch3Pin),
      elevatorZero(ElevatorConstants::kTopLimitSwitchPin),
      elevatorEnd(ElevatorConstants::kBottomLimitSwitchPin),
      actuatorTemp(DriverConstants::kHytorcThermistorPin),
      actuatorCurrent(DriverConstants::kHytorcCurrentPin),
      elevatorCurrent(ElevatorConstants::kCurrentPin),
      // TODO: these shouldn't be called with 0s, I think
      elevatorController(ElevatorConstants::kP, ElevatorConstants::kI, ElevatorConstants::kD,
                         GetElevatorFeedforward(ElevatorConstants::kS, ElevatorConstants::kV, ElevatorConstants::kA, ElevatorConstants::kG, 0, 0)),
      actuatorController(DriverConstants::kP, DriverConstants::kI, DriverConstants::kD,
                         GetSimpleFeedforward(DriverConstants::kS, DriverConstants::kV, DriverConstants::kA, 0, 0)),
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
    input = elevatorController.Run(feedback, setpoint);

    if (input < kElevatorMinVoltage) {  // voltage drop
        logger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorStuckVoltage,
                   "Elevator stuck: voltage was " + std::to_string(input) +
                       " and threshold was " + std::to_string(kElevatorMinVoltage));
    }
    if (!elevator.Run(input)) {
        logger.Log(Logger::Priority::MinorWarning, Logger::ErrorCode::InputOutOfBounds,
                   "Elevator input out of bounds: input was " + std::to_string(input));
    }

    double pos = elevator.GetPosition();

    delay(kElevatorLoopDelay);

    if (elevatorEnd.Pushed()) {
        return true;
    }
    if (elevator.GetPosition() == pos) {
        logger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorStuckPosition,
                   "Elevator stuck: position hasn't changed from " + std::to_string(pos));
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
            logger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorWrongState,
                       "Tried to zero elevator but it was already zeroed");
            return true;
        }
        state = State::ZeroElevator;
        elevator.Run(-kElevatorZeroSpeed);
    } else {
        logger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorWrongState,
                   "Tried to zero elevator, but cannot do so from state " +
                       std::to_string(static_cast<int>(state)));
        // TODO: add way to override it?
    }
    return false;
}