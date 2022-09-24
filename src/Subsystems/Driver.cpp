#include "Driver.h"

#include <tuple>

using namespace lattice;

Driver::Driver() : mLogger(Logger::logger()) {}

void Driver::Setup() {
    mElevator.Setup();
    mFirstStake.Setup();
    mSecondStake.Setup();
    mThirdStake.Setup();
    mElevatorZero.Setup();
    mElevatorEnd.Setup();
    mHandoff.setMaxSpeed(1000);
    mActuator.Setup();
}

void Driver::UpdateSensors() {
    mFirstStake.Update();
    mSecondStake.Update();
    mThirdStake.Update();
    mElevatorZero.Update();
    mElevatorEnd.Update();
    mHandoff.runSpeed();
}

void Driver::SetDriverPower(double power) {
    mActuator.SetPercentOutput(power);
}

double Driver::BoundPower(double power) {
    if (power > 0 && mElevatorZero.Get()) {
        return 0;
    } else if (power < 0 && mElevatorEnd.Get()) {
        return 0;
    } else {
        return power;
    }
}

void Driver::SetElevatorPower(double power) {
    mElevator.Run(BoundPower(power));
}
void Driver::SetDriverVoltage(double voltage) {
    mActuator.SetVoltage(voltage, GetBatteryVoltage());
}
void Driver::SetElevatorVoltage(double voltage) {
    mElevator.SetVoltage(voltage, GetBatteryVoltage() / 2.0);
}

void Driver::SetStake(StakeNumber stake) {
    mStakeState = stake;
}

void Driver::DriveStake() {
    SetDriverVoltage(DriverConstants::kDriverVoltage);
    SetElevatorVoltage(-ElevatorConstants::kElevatorVoltage);
}

void Driver::InitializeStakeHandoff() {
    mPowerScaler = 1.0;
    mStakeLimitSwitchContact = false;
    if (mStakeState == StakeNumber::kOne) {
        mHandoffDir = -1;
    } else if (mStakeState == StakeNumber::kTwo) {
        mHandoffDir = 1;
    } else if (mStakeState == StakeNumber::kThree) {
        mHandoffDir = 1;
    } else {
        // TODO Log unknown stake/
    }
}

// Add a way to run handoff (note you need to call InitializeStakeHandoff once before running the handoff)
bool Driver::RunStakeHandoff() {
    if (mStakeState == StakeNumber::kOne) {
        return RunHandoff(mFirstStake);
    } else if (mStakeState == StakeNumber::kTwo) {
        return RunHandoff(mSecondStake);
    } else if (mStakeState == StakeNumber::kThree) {
        return RunHandoff(mThirdStake);
    } else {
        // TODO Log unknown stake
    }

    return false;
}

bool Driver::RunHandoff(LimitSwitch& targetLimitSwitch) {
    if (targetLimitSwitch.Get()) {
        mHandoff.setSpeed(0.0);
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
        mHandoff.setSpeed(kHandoffSpeed * mPowerScaler * mHandoffDir);
        mHandoff.runSpeed();
    }
    return false;
}

void Driver::EStop() {
    mElevator.SetBrake(true);
    mHandoff.stop();
    mActuator.SetPercentOutput(0.0);
}
bool Driver::ZeroElevator() {
    if (mElevatorZero.Get()) {
        mElevator.Run(0);  // stop
        return true;
    } else {
        mElevator.Run(-kElevatorZeroSpeed);
        return false;
    }
}