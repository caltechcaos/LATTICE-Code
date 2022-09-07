#include "Shuttle.h"

#include <cmath>

#include "TakeupUtil.h"

using namespace lattice;

Shuttle::Shuttle() {
}
void Shuttle::Setup() {
    mLeftPassiveMotionMotor.Setup();
    mRightPassiveMotionMotor.Setup();
    mLeftDriveMotionMotor.Setup();
    mRightDriveMotionMotor.Setup();

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
        if (mArmTransitionIntermediary >= kArmTransitionIntervals) {  // This assumes that the decrements, etc were valid.
            // Since this is the state in which the setpoints get calculated, if it starts with the final state already calculated, this means we have reached our desired state.
            mArmTransitionState = kComplete;
            return true;
        }

        mArmTransitionMainAngle += mArmTransitionInterval;
        mArmTransitionOtherAngle = GetCorrespondingAngle(mTargetTakeup, kLengthArm, kDistancePassivePulleyArmPulley, kDistancePassivePulleyDrivePulley, mArmTransitionMainAngle);
        if (isnan(mArmTransitionOtherAngle)) {
            mArmTransitionOtherAngle = 0.0;
        }
        mArmTransitionState = kRunningControls;
        mArmTransitionIntermediary++;

        // Serial.print("Transition: ");
        // Serial.print(mArmTransitionInterval);
        // Serial.print(", ");
        // Serial.print("Setpoint: ");
        // Serial.print(mainArmSetpoint);
        // Serial.print("Current: ");
        // Serial.print(mArmTransitionMainAngle);
        // Serial.print("Count: ");
        // Serial.print(mArmTransitionIntermediary);
        // Serial.println();
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

void Shuttle::SetMotionMotors(double passiveLeft, double driveLeft, double driveRight, double passiveRight) {
    mLeftPassiveMotionMotor.EnableMotor();
    mRightPassiveMotionMotor.EnableMotor();
    mLeftDriveMotionMotor.EnableMotor();
    mRightDriveMotionMotor.EnableMotor();
    mLeftPassiveMotionMotor.SetVelocity(passiveLeft);
    mRightPassiveMotionMotor.SetVelocity(passiveRight);
    mLeftDriveMotionMotor.SetVelocity(driveLeft);
    mRightDriveMotionMotor.SetVelocity(driveRight);
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
    if (mLeftArmLimitSwitch.Get() && leftArmPower < 0) {
        mLeftTensionMotor.SetPercentOutput(0.0);
        if (leftArmPower > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    } else {
        mLeftTensionMotor.SetVoltage(leftArmPower, batt);  // TODO Make this voltage with the battery
        if (leftArmPower > 0) {
            leftPassive = ShuttleConstants::kTensionRPM;
            leftDrive = -ShuttleConstants::kTensionRPM;
        } else if (leftArmPower < 0) {
            leftPassive = -ShuttleConstants::kTensionRPM;
            leftDrive = ShuttleConstants::kTensionRPM;
        }
    }
    if (mRightArmLimitSwitch.Get() && rightArmPower < 0) {
        mRightTensionMotor.SetPercentOutput(0.0);
        if (rightArmPower > 0) {
            // TODO Log an error that we hit the limit switch prematurely
        }
    } else {
        mRightTensionMotor.SetVoltage(rightArmPower, batt);  // TODO Make this voltage with the battery
        if (rightArmPower > 0) {
            rightPassive = ShuttleConstants::kTensionRPM;
            rightDrive = -ShuttleConstants::kTensionRPM;
        } else if (rightArmPower < 0) {
            rightPassive = -ShuttleConstants::kTensionRPM;
            rightDrive = ShuttleConstants::kTensionRPM;
        }
    }
    SetMotionMotors(leftPassive, leftDrive, rightDrive, rightPassive);
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
        SetTensionArmPowers(-leftArm, rightArm);
    }

    return atTarget;
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
    mLeftPassiveMotionMotor.DisableMotor();
    mRightPassiveMotionMotor.DisableMotor();
    mLeftDriveMotionMotor.DisableMotor();
    mRightDriveMotionMotor.DisableMotor();
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
        StartArmTransition(preTransition);
    } else if (mStakeTransitionState == kLowerFrontArm) {
        // StopMotionMotors();
        if (ArmTransition(preTransition)) {
            mStakeTransitionState = kGoingOnRail;
        }
    } else if (mStakeTransitionState == kGoingOnRail) {
        if (!mCenterLimitSwitch.Get()) {
            // SetMotionMotors(kRailRPM);
        } else {
            // SetMotionMotors(0.0);
            mStakeTransitionState = kCenterOfRail;
            StartArmTransition(postTransition);
        }

    } else if (mStakeTransitionState == kCenterOfRail) {
        // SetMotionMotors(0.0);
        if (ArmTransition(postTransition)) {
            mStakeTransitionState = kGoingOffRail;
        }
    } else if (mStakeTransitionState == kGoingOffRail) {
        if ((mFrontLimitSwitch == kLeft && !mRightLimitSwitch.Get()) || (mFrontLimitSwitch == kRight && !mLeftLimitSwitch.Get())) {
            // SetMotionMotors(kRailRPM);
        } else {
            // StopMotionMotors();
            StartArmTransition(kBothArmsRaised);
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
    if (mLeftLimitSwitch.Get() || mRightLimitSwitch.Get()) {
        // StopMotionMotors();
        if ((mLeftLimitSwitch.Get() && mFrontLimitSwitch == kLeft) || (mRightLimitSwitch.Get() && mFrontLimitSwitch == kRight)) {  // If front limit switch is hit start stake transition
            mStakeTransitionState = kHitRail;
            return true;
        } else {
            // TODO: Log that something isn't right most likely that the front of the robot wasn't configured properly
        }
    } else {
        // SetMotionMotors(kDriveRPM);
    }
}
void Shuttle::EStop() {
    StopMotionMotors();

    mLeftTensionMotor.SetPercentOutput(0);
    mRightTensionMotor.SetPercentOutput(0);

    // Engage Motor break to prevent arms from moving
    EngageMotorBreak();
}