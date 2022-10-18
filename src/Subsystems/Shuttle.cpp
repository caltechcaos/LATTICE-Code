#include "Shuttle.h"

#include <cmath>

#include "TakeupUtil.h"

using namespace lattice;

Shuttle::Shuttle() {
}
void Shuttle::Setup() {
    mOuterLeftMotionMotor.Setup();
    mOuterRightMotionMotor.Setup();
    mInnerLeftMotionMotor.Setup();
    mInnerRightMotionMotor.Setup();

    mLeftTensionMotor.Setup();
    mRightTensionMotor.Setup();

    mLeftArmBottomLimitSwitch.Setup();
    mRightArmBottomLimitSwitch.Setup();

    mArmBrake.Setup();

    mCenterSwitch.Setup();

    mBatterySensor.Setup();
}
void Shuttle::SetTakeup(double takeup) {
    mTargetTakeup = takeup;
    mOneArmTakeupAngle = SingleArmTakeupAngle(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley);
    mTwoArmTakeupAngle = DoubleArmTakeupAngles(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley);

    // If we want to enforce a maximum difference in the change in angle per step of the arm transition:
    mArmTransitionIntervals = (int)ceil((mOneArmTakeupAngle - mTwoArmTakeupAngle) / kMaxDegreeIncrement);
}

void Shuttle::SetFrontLimitSwitch(FrontLimitSwitch front) {
    mFrontLimitSwitch = front;
}

void Shuttle::SetMotion(double RPM) {
    SetMotionMotors(RPM, RPM, RPM, RPM);
}

bool Shuttle::ArmTransition(ArmTransitionPositions pos) {
    if (pos == kBothArmsRaisedNormal) {
        return SetTensionArmPositions(mTwoArmTakeupAngle, mTwoArmTakeupAngle);
    }
    // Lower or raise an arm to a desired angle with a certain increment and calculate the corresponding angle of the other arm.
    if (mArmTransitionState == kCalculate) {
        double mainArmSetpoint;
        if (pos == kLeftArmRaised || pos == kRightArmRaised) {
            mainArmSetpoint = mOneArmTakeupAngle;
        } else {
            mainArmSetpoint = mTwoArmTakeupAngle;
        }
        if (mArmTransitionIntermediary >= mArmTransitionIntervals) {  // This assumes that the decrements, etc were valid.
            // Since this is the state in which the setpoints get calculated, if it starts with the final state already calculated, this means we have reached our desired state.
            mArmTransitionState = kComplete;
            return true;
        }

        mArmTransitionMainAngle += mArmTransitionIncrement;
        mArmTransitionOtherAngle = GetCorrespondingAngle(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley, mArmTransitionMainAngle);
        if (isnan(mArmTransitionOtherAngle)) {
            mArmTransitionOtherAngle = 0.0;
        }
        mArmTransitionState = kRunningControls;
        mArmTransitionIntermediary++;
    } else if (mArmTransitionState == kRunningControls) {
        bool atPosition;
        if (pos == kLeftArmRaised) {
            atPosition = SetTensionArmPositions(mArmTransitionMainAngle, mArmTransitionOtherAngle);
        } else if (pos == kRightArmRaised) {
            atPosition = SetTensionArmPositions(mArmTransitionOtherAngle, mArmTransitionMainAngle);
        } else if (pos == kBothArmsRaised) {
            if (mLeftTensionMotor.GetPosition() < mArmTransitionMainAngle) {  // Since we're incrementing up, for the case of both arms, we need to find the arm thats lower than the setpoint.
                atPosition = SetTensionArmPositions(mArmTransitionOtherAngle, mArmTransitionMainAngle);
            } else {
                atPosition = SetTensionArmPositions(mArmTransitionMainAngle, mArmTransitionOtherAngle);
            }
        }
        if (atPosition) {
            mArmTransitionState = kCalculate;
        }
    } else if (mArmTransitionState == kComplete) {
        return true;
    } else {
        // Log an error of unknown state
    }
    return false;
}
void Shuttle::UpdateSensors() {
    mLeftArmBottomLimitSwitch.Update();
    mRightArmBottomLimitSwitch.Update();
}

void Shuttle::EngageMotorBreak() {
    mArmBrake.Set(true);
}

void Shuttle::DisengageMotorBreak() {
    mArmBrake.Set(false);
}

void Shuttle::SetMotionMotors(double outerLeft, double innerLeft, double innerRight, double outerRight) {
    mOuterLeftMotionMotor.EnableMotor();
    mOuterRightMotionMotor.EnableMotor();
    mInnerLeftMotionMotor.EnableMotor();
    mInnerRightMotionMotor.EnableMotor();
    mOuterLeftMotionMotor.SetVelocity(outerLeft);
    mOuterRightMotionMotor.SetVelocity(outerRight);
    mInnerLeftMotionMotor.SetVelocity(innerLeft);
    mInnerRightMotionMotor.SetVelocity(innerRight);
}

double Shuttle::BoundPID(double input, double minimum, double target, double pos) {
    double error = target - pos;
    if (((input >= 0) && (error >= 0)) || ((input < 0) && (error < 0))) {
        if (abs(input) < abs(minimum)) {
            return copysign(minimum, error);
        } else {
            return input;
        }

    } else {
        return copysign(minimum, error);
    }
}

void Shuttle::SetTensionArmPowers(double leftArmPower, double rightArmPower) {
    auto batt = GetBatteryVoltage();
    double leftPassive, leftDrive, rightDrive, rightPassive = 0.0;
    if ((mLeftArmBottomLimitSwitch.Get() && leftArmPower < 0)) {
        mLeftTensionMotor.SetPercentOutput(0.0);
        if (mLeftArmBottomLimitSwitch.Get()) {
            mLeftTensionMotor.ResetEncoderPosition();
        }
        if (abs(leftArmPower) > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    } else {
        mLeftTensionMotor.SetVoltage(-leftArmPower, batt);  // TODO Make this voltage with the battery
        if (leftArmPower > 0) {
            leftPassive = ShuttleConstants::kTensionRPM;
        } else if (leftArmPower < 0) {
            leftPassive = -ShuttleConstants::kTensionRPM;
        }
    }
    if ((mRightArmBottomLimitSwitch.Get() && rightArmPower < 0)) {
        mRightTensionMotor.SetPercentOutput(0.0);
        if (mRightArmBottomLimitSwitch.Get()) {
            mRightTensionMotor.ResetEncoderPosition();
        }
        if (rightArmPower > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    } else {
        mRightTensionMotor.SetVoltage(rightArmPower, batt);  // TODO Make this voltage with the battery
        if (rightArmPower > 0) {
            rightPassive = -ShuttleConstants::kTensionRPM;
        } else if (rightArmPower < 0) {
            rightPassive = ShuttleConstants::kTensionRPM;
        }
    }
    SetMotionMotors(leftPassive, 0, 0, rightPassive);
}

bool Shuttle::SetTensionArmPositions(double leftArmAngle, double rightArmAngle) {
    double leftArmPos = GetLeftTensionArmPos();
    double rightArmPos = GetRightTensionArmPos();
    double leftArm = BoundPID(mLeftArmController.Run(leftArmPos, leftArmAngle), ShuttleConstants::kS, leftArmAngle, leftArmPos);
    double rightArm = BoundPID(mRightArmController.Run(rightArmPos, rightArmAngle), ShuttleConstants::kS, rightArmAngle, rightArmPos);
    bool atTarget = mLeftArmController.AtTarget() && mRightArmController.AtTarget();

    if (atTarget) {
        SetTensionArmPowers(0, 0);
        SetMotionMotors(0, 0, 0, 0);
    } else {
        SetTensionArmPowers(leftArm, rightArm);
    }

    return atTarget;
}
void Shuttle::ResetTensionArms() {
    SetTensionArmPowers(-kPercentPowerReset * GetBatteryVoltage(), -kPercentPowerReset * GetBatteryVoltage());
}
void Shuttle::StopMotionMotors() {
    mOuterLeftMotionMotor.DisableMotor();
    mOuterRightMotionMotor.DisableMotor();
    mInnerLeftMotionMotor.DisableMotor();
    mInnerRightMotionMotor.DisableMotor();
}

void Shuttle::StartArmTransition(ArmTransitionPositions pos) {
    mArmTransitionState = ArmTransitionState::kCalculate;
    mArmTransitionIntermediary = 0;
    if (pos == ArmTransitionPositions::kBothArmsRaised) {
        mArmTransitionMainAngle = GetLeftTensionArmPos();
        mArmTransitionIncrement = (mTwoArmTakeupAngle - GetLeftTensionArmPos()) / mArmTransitionIntervals;
    } else if (pos == ArmTransitionPositions::kLeftArmRaised) {
        mArmTransitionMainAngle = GetLeftTensionArmPos();
        mArmTransitionIncrement = (mOneArmTakeupAngle - GetLeftTensionArmPos()) / mArmTransitionIntervals;
    } else if (pos == ArmTransitionPositions::kRightArmRaised) {
        mArmTransitionMainAngle = GetRightTensionArmPos();
        mArmTransitionIncrement = (mOneArmTakeupAngle - GetRightTensionArmPos()) / mArmTransitionIntervals;
    }
}

double Shuttle::GetBatteryVoltage() {
    return mBatterySensor.CalculateFilteredVoltage();
}

bool Shuttle::StakeTransition(bool offRail) {
    ArmTransitionPositions preTransition;
    ArmTransitionPositions postTransition;
    if (mFrontLimitSwitch == kLeft) {
        preTransition = kRightArmRaised;
        postTransition = kLeftArmRaised;
    } else {  // Right side is front
        preTransition = kLeftArmRaised;
        postTransition = kRightArmRaised;
    }
    if (mStakeTransitionState == kHitRail) {
        mStakeTransitionState = kLowerFrontArm;
        StartArmTransition(preTransition);
        DisengageMotorBreak();
    } else if (mStakeTransitionState == kLowerFrontArm) {
        StopMotionMotors();
        DisengageMotorBreak();
        if (ArmTransition(preTransition)) {
            mStakeTransitionState = kGoingOnRail;
        }
    } else if (mStakeTransitionState == kGoingOnRail) {
        if (!mCenterSwitch.Get()) {
            SetMotion(kRailRPM);
            EngageMotorBreak();
        } else {
            SetMotion(0.0);
            mStakeTransitionState = kCenterOfRail;
            StartArmTransition(postTransition);
            DisengageMotorBreak();
        }

    } else if (mStakeTransitionState == kCenterOfRail) {
        SetMotion(0.0);
        DisengageMotorBreak();
        if (ArmTransition(postTransition)) {
            mStakeTransitionState = kGoingOffRail;
        }
    } else if (mStakeTransitionState == kGoingOffRail) {
        if (!offRail) {
            EngageMotorBreak();
            SetMotion(kRailRPM);
        } else {
            StopMotionMotors();
            DisengageMotorBreak();
            StartArmTransition(kBothArmsRaised);
            mStakeTransitionState = kResettingArms;
        }
    } else if (mStakeTransitionState == kResettingArms) {
        DisengageMotorBreak();
        if (ArmTransition(kBothArmsRaised)) {
            mStakeTransitionState = kDone;
        }
    } else if (mStakeTransitionState == kDone) {
        return true;
    } else {
        // Error
    }
    return false;
}

bool Shuttle::ConstantTakeupDrive(bool hitFront) {
    // At the very start of an autonomous run, take up the cable
    if (mAtStart) {
        DisengageMotorBreak();
        if (ArmTransition(Shuttle::ArmTransitionPositions::kBothArmsRaisedNormal)) {
            mAtStart = false;
        }
        return false;
    }
    if (hitFront) {
        mStakeTransitionState = kHitRail;
        return true;
    } else {
        SetMotion(kDriveRPM);
        EngageMotorBreak();
    }
    return false;
}
void Shuttle::EStop() {
    StopMotionMotors();

    mLeftTensionMotor.SetPercentOutput(0);
    mRightTensionMotor.SetPercentOutput(0);
}