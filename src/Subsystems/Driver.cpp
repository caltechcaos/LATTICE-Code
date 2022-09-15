#include "Driver.h"

#include <tuple>

#include "FeedforwardUtil.h"
#include "Util.h"

using namespace lattice;

Driver::Driver()
    : mElevator(ElevatorConstants::kMotorPin, ElevatorConstants::kEncoderFwdPin, ElevatorConstants::kEncoderBckPin),
      mActuator(DriverConstants::kHytorcMotorPin, DriverConstants::kHytorcEncoderForward, DriverConstants::kHytorcEncoderBackward),
      mHandoff(HandoffConstants::kStepsPerRev, HandoffConstants::kStepPin1, HandoffConstants::kStepPin2, HandoffConstants::kStepPin3, HandoffConstants::kStepPin4),
      /* rc_input() */
      mLogger(Logger::logger()),
      mFirstStake(HandoffConstants::kLimitSwitch1Pin),
      mSecondStake(HandoffConstants::kLimitSwitch2Pin),
      mThirdStake(HandoffConstants::kLimitSwitch3Pin),
      mElevatorZero(ElevatorConstants::kTopLimitSwitchPin),
      mElevatorEnd(ElevatorConstants::kBottomLimitSwitchPin),
      mActuatorTemp(DriverConstants::kHytorcThermistorPin),
      mActuatorCurrent(DriverConstants::kHytorcCurrentPin),
      mElevatorCurrent(ElevatorConstants::kCurrentPin),
      // TODO: these shouldn't be called with 0s, I think
      mElevatorController(ElevatorConstants::kP, ElevatorConstants::kI, ElevatorConstants::kD,
                          GetElevatorFeedforward(ElevatorConstants::kS, ElevatorConstants::kV, ElevatorConstants::kA, ElevatorConstants::kG, 0, 0)),
      mActuatorController(DriverConstants::kP, DriverConstants::kI, DriverConstants::kD,
                          GetSimpleFeedforward(DriverConstants::kS, DriverConstants::kV, DriverConstants::kA, 0, 0)),
      mState(DriverState::kIdle) {}

void Driver::Setup() {
    mElevator.Setup();
    mFirstStake.Setup();
    mSecondStake.Setup();
    mThirdStake.Setup();
    mElevatorZero.Setup();
    mElevatorEnd.Setup();
}

void Driver::UpdateSensors() {
    mFirstStake.Update();
    mSecondStake.Update();
    mThirdStake.Update();
    mElevatorZero.Update();
    mElevatorEnd.Update();
}

void Driver::Run() {
    UpdateSensors();
    if (mState == DriverState::kIdle) {
        mHandoff.setSpeed(0.0);
        mElevator.SetVoltage(0, 1);
    } else if (mState == DriverState::kManual) {
    } else if (mState == DriverState::kHandoff) {
    } else if (mState == DriverState::kAutoDrive) {
    } else if (mState == DriverState::kZeroElevator) {
    } else {
        // Invalid state
    }
}

void Driver::SetDriverPower(double power) {
    mActuator.SetPercentOutput(power);
}

void Driver::SetElevatorPower(double power) {
    mElevator.Run(power);
}

bool Driver::SetStake(StakeNumber stake) {
    mStakeState = stake;
}

void Driver::DriveStake() {
    // TODO Implement
}

void Driver::InitializeStakeHandoff() {
    mPowerScaler = 1.0;
    if (mStakeState == StakeNumber::kOne) {
        if (mFirstStake.Get()) {
            return;
        } else if (mSecondStake.Get()) {
            mHandoffDir = 1;
        } else if (mThirdStake.Get()) {
            mHandoffDir = -1;
        } else {
            // LOG Error because we don't know where it is
        }
    } else if (mStakeState == StakeNumber::kTwo) {
        mHandoffDir = -1;
    } else if (mStakeState == StakeNumber::kThree) {
        mHandoffDir = 1;
    } else {
        // TODO Log unknown stake/
    }
}

bool Driver::RunStakeHandoff() {
    if (mStakeState == StakeNumber::kOne) {
        return RunStakeHandoff(mFirstStake);
    } else if (mStakeState == StakeNumber::kTwo) {
        return RunStakeHandoff(mSecondStake);
    } else if (mStakeState == StakeNumber::kThree) {
        return RunStakeHandoff(mThirdStake);
    } else {
        // TODO Log unknown stake/
    }
    return false;
}

bool Driver::RunStakeHandoff(LimitSwitch& targetLimitSwitch) {
    if (targetLimitSwitch.Get()) {
        mHandoff.setSpeed(0.0);
        mHandoff.step(0);
        if (mStakeLimitSwitchContact) {
            return true;
        } else {
            mStakeLimitSwitchContact = true;
        }
    } else {
        if (mStakeLimitSwitchContact) {
            // If we overshot, we go half as fast in the other direction
            mHandoffDir *= -1;
            mPowerScaler *= 0.5;
            mStakeLimitSwitchContact = false;
        }
        mHandoff.setSpeed(kHandoffSpeed * mPowerScaler);
        mHandoff.step(mHandoffDir * 10000000);
    }
    return false;
}

void Driver::EStop() {
    mElevator.SetBrake(true);
    // actuator.SetBrake(true);
    mHandoff.step(0);
    mHandoff.setSpeed(0);
}

bool Driver::RunElevatorOneTick(double setpoint) {
    double feedback = mElevatorCurrent.Get();

    double input;
    input = mElevatorController.Run(feedback, setpoint);

    if (input < kElevatorMinVoltage) {  // voltage drop
        mLogger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorStuckVoltage,
                    "Elevator stuck: voltage was " + std::to_string(input) +
                        " and threshold was " + std::to_string(kElevatorMinVoltage));
    }
    if (!mElevator.Run(input)) {
        mLogger.Log(Logger::Priority::MinorWarning, Logger::ErrorCode::InputOutOfBounds,
                    "Elevator input out of bounds: input was " + std::to_string(input));
    }

    double pos = mElevator.GetPosition();

    if (mElevatorEnd.Pushed()) {
        return true;
    }
    if (mElevator.GetPosition() == pos) {
        mLogger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorStuckPosition,
                    "Elevator stuck: position hasn't changed from " + std::to_string(pos));
    }

    return false;
}

bool Driver::ZeroElevator() {
    if (mState == DriverState::kZeroElevator) {
        if (mElevatorZero.Get()) {
            mElevator.Run(0);  // stop
            mState = DriverState::kIdle;
            return true;
        }
    } else if (mState == DriverState::kIdle) {
        if (mElevatorZero.Get()) {
            mLogger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorWrongState,
                        "Tried to zero elevator but it was already zeroed");
            return true;
        }
        mState = DriverState::kZeroElevator;
        mElevator.Run(-kElevatorZeroSpeed);
    } else {
        mLogger.Log(Logger::Priority::Warning, Logger::ErrorCode::ElevatorWrongState,
                    "Tried to zero elevator, but cannot do so from state " +
                        std::to_string(static_cast<int>(mState)));
        // TODO: add way to override it?
    }
    return false;
}