#include "Shuttle.h"

#include "TakeupUtil.h"

using namespace lattice;

Shuttle::Shuttle() {
}
void Shuttle::Setup() {
    mLeftMotionMotor.Setup();
    mRightMotionMotor.Setup();

    mLeftTensionMotor.Setup();
    mRightTensionMotor.Setup();

    mLeftArmLimitSwitch.Setup();
    mRightArmLimitSwitch.Setup();

    mLeftLimitSwitch.Setup();
    mRightLimitSwitch.Setup();
    mCenterLimitSwitch.Setup();
}
void Shuttle::SetTakeup(double takeup) {
    mTargetTakeup = takeup;
    mOneArmTakeupAngle = SingleArmTakeupAngle(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley);
    mTwoArmTakeupAngle = DoubleArmTakeupAngles(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley);
}

void Shuttle::SetFrontLimitSwitch(FrontLimitSwitch front) {
    mFrontLimitSwitch = front;
}

bool Shuttle::ArmTransition(ArmTransitionPositions pos) {
    // Lower or raise an arm to a desired angle with a certain increment and calculate the corresponding angle of the other arm.
    if (mArmTransitionState == kCalculate) {
        double mainArmSetpoint;
        if (pos == kLeftArmRaised || pos == kRightArmRaised) {
            mainArmSetpoint = mOneArmTakeupAngle;
        } else {
            mainArmSetpoint = mTwoArmTakeupAngle;
        }
        if (abs(mainArmSetpoint - mArmTransitionMainAngle) < 0.1) {  // This assumes that the decrements, etc were valid.
            // Since this is the state in which the setpoints get calculated, if it starts with the final state already calculated, this means we have reached our desired state.
            mArmTransitionState = kComplete;
        }

        mArmTransitionMainAngle += mArmTransitionInterval;
        mArmTransitionOtherAngle = GetCorrespondingAngle(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley, mArmTransitionMainAngle);
        mArmTransitionState = kRunningControls;
    } else if (mArmTransitionState == kRunningControls) {
        bool atPosition;
        if (pos == kLeftArmRaised) {
            atPosition = SetTensionArmPositions(mArmTransitionMainAngle, mArmTransitionOtherAngle);
        } else if (pos == kRightArmRaised) {
            atPosition = SetTensionArmPositions(mArmTransitionOtherAngle, mArmTransitionMainAngle);
        } else if (pos == kBothArmsRaised) {
            if (mLeftTensionMotor.GetPosition() < mArmTransitionMainAngle) {  // Since we're incrementing up, for the case of both arms, we need to find the arm thats lower than the setpoint.
                atPosition = SetTensionArmPositions(mArmTransitionMainAngle, mArmTransitionOtherAngle);
            } else {
                atPosition = SetTensionArmPositions(mArmTransitionOtherAngle, mArmTransitionMainAngle);
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
    mLeftArmLimitSwitch.Update();
    mRightArmLimitSwitch.Update();

    mLeftLimitSwitch.Update();
    mRightLimitSwitch.Update();
    mCenterLimitSwitch.Update();
}

void Shuttle::EngageMotorBreak() {
    // TODO
}

void Shuttle::DisengageMotorBreak() {
    // TODO
}

void Shuttle::SetMotionMotors(double RPM) {
    mLeftMotionMotor.EnableMotor();
    mRightMotionMotor.EnableMotor();
    mLeftMotionMotor.SetVelocity(RPM);
    mRightMotionMotor.SetVelocity(RPM);
}

void Shuttle::SetTensionArmPowers(double leftArmPower, double rightArmPower) {
    auto batt = GetBatteryVoltage();
    if (!mLeftArmLimitSwitch.Get()) {
        mLeftTensionMotor.SetVoltage(leftArmPower, batt);  // TODO Make this voltage with the battery
    } else {
        mLeftTensionMotor.SetPercentOutput(0.0);
        if (leftArmPower > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    }
    if (!mRightArmLimitSwitch.Get()) {
        mRightTensionMotor.SetVoltage(rightArmPower, batt);  // TODO Make this voltage with the battery
    } else {
        mRightTensionMotor.SetPercentOutput(0.0);
        if (rightArmPower > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    }
}

bool Shuttle::SetTensionArmPositions(double leftArmAngle, double rightArmAngle) {
    SetTensionArmPowers(mLeftArmController.Run(GetLeftTensionArmPos(), leftArmAngle), mRightArmController.Run(GetRightTensionArmPos(), rightArmAngle));
    return mLeftArmController.AtTarget() && mRightArmController.AtTarget();
}
void Shuttle::ResetTensionArms() {
    if (!mLeftArmLimitSwitch.Get()) {
        mLeftTensionMotor.SetPercentOutput(-kPercentPowerReset);
    } else {
        mLeftTensionMotor.SetPercentOutput(0.0);
    }

    if (!mRightArmLimitSwitch.Get()) {
        mRightTensionMotor.SetPercentOutput(-kPercentPowerReset);
    } else {
        mRightTensionMotor.SetPercentOutput(0.0);
    }
}
void Shuttle::StopMotionMotors() {
    mLeftMotionMotor.DisableMotor();
    mRightMotionMotor.DisableMotor();
}

bool Shuttle::StakeTransition() {
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
        mArmTransitionIntermediary = 0;
        mArmTransitionMainAngle = mLeftTensionMotor.GetPosition();
        mArmTransitionInterval = (mOneArmTakeupAngle - mLeftTensionMotor.GetPosition()) / kArmTransitionIntervals;
    } else if (mStakeTransitionState == kLowerFrontArm) {
        StopMotionMotors();
        if (ArmTransition(preTransition)) {
            mStakeTransitionState = kGoingOnRail;
        }
    } else if (mStakeTransitionState == kGoingOnRail) {
        if (!mCenterLimitSwitch.Get()) {
            SetMotionMotors(kRailRPM);
        } else {
            SetMotionMotors(0.0);
            mStakeTransitionState = kCenterOfRail;
        }

    } else if (mStakeTransitionState == kCenterOfRail) {
        SetMotionMotors(0.0);
        if (ArmTransition(postTransition)) {
            mStakeTransitionState = kGoingOffRail;
        }
    } else if (mStakeTransitionState == kGoingOffRail) {
        if ((mFrontLimitSwitch == kLeft && !mRightLimitSwitch.Get()) || (mFrontLimitSwitch == kRight && !mLeftLimitSwitch.Get())) {
            SetMotionMotors(kRailRPM);
        } else {
            StopMotionMotors();
            mStakeTransitionState = kResettingArms;
        }
    } else if (mStakeTransitionState == kResettingArms) {
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

bool Shuttle::ConstantTakeupDrive() {
    if (mLeftLimitSwitch.Get() || mRightLimitSwitch.Get() || mTargetArmPos != kBothArmsRaised) {
        StopMotionMotors();
        if (mTargetArmPos != kBothArmsRaised) {
            // TODO: Log somewhere that the arms aren't ready for a constant takeup drive
        } else if ((mLeftLimitSwitch.Get() && mFrontLimitSwitch == kLeft) || (mRightLimitSwitch.Get() && mFrontLimitSwitch == kRight)) {  // If front limit switch is hit start stake transition
            mStakeTransitionState = kHitRail;
            return true;
        } else {
            // TODO: Log that something isn't right most likely that the front of the robot wasn't configured properly
        }
    } else {
        SetMotionMotors(kDriveRPM);
    }
}
void Shuttle::EStop() {
    StopMotionMotors();

    mLeftTensionMotor.SetPercentOutput(0);
    mRightTensionMotor.SetPercentOutput(0);

    // Engage Motor break to prevent arms from moving
    EngageMotorBreak();
}