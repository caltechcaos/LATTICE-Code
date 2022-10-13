#pragma once
#include "Brake.h"
#include "HallEffect.h"
#include "HytorcSimple.h"
#include "LimitSwitch.h"
#include "MotionMotor.h"
#include "PIDF.h"
#include "Util.h"
#include "Voltage.h"

namespace lattice {
/**
 * Class with shuttle controls
 */
class Shuttle {
   public:
    /**
     * These are the states that the tensioning arms can be at during arm transition.
     */
    enum ArmTransitionPositions {
        kLeftArmRaised,
        kRightArmRaised,
        kBothArmsRaised,
        kBothArmsRaisedNormal
    };

    /**
     * Possible limit switches to be considered the "front" of the shuttle.
     */
    enum FrontLimitSwitch {
        kLeft,
        kRight
    };

    /**
     * The different states for the stake transition
     */
    enum StakeTransitionState {
        kHitRail,
        kLowerFrontArm,
        kGoingOnRail,
        kCenterOfRail,
        kGoingOffRail,
        kResettingArms,
        kDone
    };

    enum ArmTransitionState {
        kCalculate,
        kRunningControls,
        kComplete
    };

    /**
     * Singleton to ensure we only have one shuttle subsystem floating around
     * @returns The single instance of the shuttle
     */
    static Shuttle& GetInstance() {
        static Shuttle shuttle;
        return shuttle;
    }

    /**
     * Setup function for the shuttle
     */
    void Setup();

    /**
     * Set takeup value for the shuttle. This will allow the shuttle to calculate the necessary set positions for the tensioning arms.
     *
     * @param takeup The desired takeup in meters
     */
    void SetTakeup(double takeup);

    /**
     * Set front limit switch for the shuttle.
     * This is the limit switch that will be hit first when the shuttle makes contact with a state rail during its path.
     *
     * @param front The front limit switch to configure to
     */
    void SetFrontLimitSwitch(FrontLimitSwitch front);

    /**
     * Transitions the arms from one arm position to the other
     *
     * @param pos The arm position to be at
     * @return True if the arms are at the desired position
     */
    bool ArmTransition(ArmTransitionPositions pos);

    /**
     * Updates the shuttle sensors
     */
    void UpdateSensors();

    /**
     * Sets the motion motors to move
     *
     * @param outerLeft The RPM the outer left pulley should move at
     * @param innerLeft The RPM the inner left pulley should move at
     * @param innerRight The RPM the inner right pulley should move at
     * @param outerRight The RPM the outer right pulley should move at
     */
    void SetMotionMotors(double outerLeft, double innerLeft, double innerRight, double outerRight);

    /**
     * Sets the shuttle to move in a direction with a specified rpm
     *
     * @param RPM The RPM in which to move at
     */
    void SetMotion(double RPM);

    /**
     * Resets the tensioning arms by zeroing with a limit switch
     */
    void ResetTensionArms();

    void SetTensionArmPowers(double leftArmPower, double rightArmPower);

    /**
     * Engages the motor break
     */
    void EngageMotorBreak();

    /**
     * Disengages the motor break
     */
    void DisengageMotorBreak();

    /**
     * Stops the motion motors completely by disabling them. Note that normally if you set the RPM to 0, the motor will try to stay at 0 RPM.
     */
    void StopMotionMotors();

    /**
     * Handles the logic of performing the stake transition
     *
     * @return True if the stake transition is complete
     */
    bool StakeTransition(bool offRail);

    /**
     * Runs constant takeup traversal across cable. Only runs if arms are already configured to be both raised.
     *
     * @return True if the stake transition should start
     */
    bool ConstantTakeupDrive(bool hitFront);

    /**
     * Emergency stop that freezes all the actuators.
     */
    void EStop();

    /**
     * Sets the tensioning arm to target angles. Note that ArmTransition should be used for any final, autonomous code
     *
     * @param leftArmAngle The angle the left arm should be at
     * @param rightArmAngle The angle the right arm should be at
     */
    bool SetTensionArmPositions(double leftArmAngle, double rightArmAngle);

    /**
     * Get the left tension arm position in degrees
     *
     * @return the left tension arm position in degrees
     */
    double GetLeftTensionArmPos() { return mLeftTensionMotor.GetPosition() * 360.0 / 5000; };  // TODO Account for gearing here officially

    /**
     * Get the right tension arm position in degrees
     *
     * @return the right tension arm position in degrees
     */
    double GetRightTensionArmPos() { return -mRightTensionMotor.GetPosition() * 360.0 / 5000; };

    void ResetArmPositions() {
        mLeftTensionMotor.ResetEncoderPosition();
        mRightTensionMotor.ResetEncoderPosition();
    };

    double GetLeftSetpoint() { return mLeftArmController.GetSetpoint(); };
    double GetRightSetpoint() { return mRightArmController.GetSetpoint(); };

    double GetBatteryVoltage();
    void StartArmTransition(ArmTransitionPositions pos);

    // Guarantee the singleton
    Shuttle(Shuttle const&) = delete;
    void operator=(Shuttle const&) = delete;

   private:
    /**
     * Constructor for the shuttle
     */
    Shuttle();

    double BoundPID(double input, double minimum, double target, double pos);

    static constexpr double kPercentPowerReset = 0.25;
    static constexpr double kDriveRPM = 5800;  // slightly less than max so that resolution issues don't lead to an invalid PWM signal
    static constexpr double kRailRPM = 3000;

    static constexpr double kDistancePassivePulleyArmPulley = 0.2;
    static constexpr double kDistancePassivePulleyDrivePulley = 0.1542;
    static constexpr double kLengthArm = 0.25;

    // // TODO Fix these numbers to an actual constants file
    MotionMotor mOuterRightMotionMotor{ShuttleConstants::kOuterRightMotionMotorEnablePin, ShuttleConstants::kOuterRightMotionMotorSignalPin, ShuttleConstants::kOuterRightMotionMotorRPMPin, ShuttleConstants::kOuterRightMotionMotorThermalPin};
    MotionMotor mOuterLeftMotionMotor{ShuttleConstants::kOuterLeftMotionMotorEnablePin, ShuttleConstants::kOuterLeftMotionMotorSignalPin, ShuttleConstants::kOuterLeftMotionMotorRPMPin, ShuttleConstants::kOuterLeftMotionMotorThermalPin};
    MotionMotor mInnerRightMotionMotor{ShuttleConstants::kInnerRightMotionMotorEnablePin, ShuttleConstants::kInnerRightMotionMotorSignalPin, ShuttleConstants::kInnerRightMotionMotorRPMPin, ShuttleConstants::kInnerRightMotionMotorThermalPin};
    MotionMotor mInnerLeftMotionMotor{ShuttleConstants::kInnerLeftMotionMotorEnablePin, ShuttleConstants::kInnerLeftMotionMotorSignalPin, ShuttleConstants::kInnerLeftMotionMotorRPMPin, ShuttleConstants::kInnerLeftMotionMotorThermalPin};

    HytorcSimple mRightTensionMotor{ShuttleConstants::kRightHytorcMotorPin, ShuttleConstants::kRightHytorcForwardEncoderPin, ShuttleConstants::kRightHytorcBackwardEncoderPin};
    HytorcSimple mLeftTensionMotor{ShuttleConstants::kLeftHytorcMotorPin, ShuttleConstants::kLeftHytorcForwardEncoderPin, ShuttleConstants::kLeftHytorcBackwardEncoderPin};

    Brake mRightBrake{ShuttleConstants::kRightArmBrakePin};
    Brake mLeftBrake{ShuttleConstants::kLeftArmBrakePin};

    Voltage mBatterySensor{ShuttleConstants::kBatteryVoltagePin};

    LimitSwitch mRightArmBottomLimitSwitch{ShuttleConstants::kRightArmBottomLimitSwitchPin};
    LimitSwitch mLeftArmBottomLimitSwitch{ShuttleConstants::kLeftArmBottomLimitSwitchPin};
    LimitSwitch mRightArmTopLimitSwitch{ShuttleConstants::kRightArmTopLimitSwitchPin};
    LimitSwitch mLeftArmTopLimitSwitch{ShuttleConstants::kLeftArmTopLimitSwitchPin};
    HallEffect mCenterSwitch{ShuttleConstants::kCenterHallSwitchPin};

    PIDF mLeftArmController{ShuttleConstants::kP, ShuttleConstants::kI, ShuttleConstants::kD, [](double setpoint) { return copysign(ShuttleConstants::kS, setpoint); }, ShuttleConstants::kError, ShuttleConstants::kVError};
    PIDF mRightArmController{ShuttleConstants::kP, ShuttleConstants::kI, ShuttleConstants::kD, [](double setpoint) { return copysign(ShuttleConstants::kS, setpoint); }, ShuttleConstants::kError, ShuttleConstants::kVError};

    bool mAtStart = true;
    double mTargetTakeup;
    double mOneArmTakeupAngle;
    double mTwoArmTakeupAngle;
    int mArmTransitionIntervals;                        // Number of intermediary states required to make the arm transition.
    static constexpr double kMaxDegreeIncrement = 5.0;  // 5 degrees should prevent a significant amount of overtensioning if an arm doesn't reach its intermediary position in time
    ArmTransitionPositions mTargetArmPos;
    FrontLimitSwitch mFrontLimitSwitch;
    StakeTransitionState mStakeTransitionState;
    ArmTransitionState mArmTransitionState = ArmTransitionState::kCalculate;
    int mArmTransitionIntermediary = 0;
    double mArmTransitionIncrement = 0;
    double mArmTransitionMainAngle = 0;
    double mArmTransitionOtherAngle = 0;
};
}  // namespace lattice