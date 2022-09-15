#pragma once
#include <AccelStepper.h>

#include "Clifford.h"
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "FeedforwardUtil.h"
#include "HytorcSimple.h"
#include "LimitSwitch.h"
#include "Logger.h"
#include "PIDF.h"
#include "Thermistor.h"
#include "Util.h"

namespace lattice {
class Driver {
   public:
    enum class StakeNumber {
        kOne,
        kTwo,
        kThree
    };

    /**
     * Singleton to ensure we only have one driver subsystem floating around
     * @returns The single instance of the driver
     */
    static Driver& driver() {
        static Driver driver;
        return driver;
    }

    /**
     * Sets up to use the driver subsystem. Must be called before
     * using it.
     */
    void Setup();

    /**
     * Advances the subsystem by one tick
     */
    void Run();

    bool SetStake(StakeNumber stake);

    bool RunStakeHandoff();

    void InitializeStakeHandoff();

    /**
     * Flips the emergency stop and freezes all actuators
     */
    void EStop();

    /**
     * Runs the zero-elevator process by one tick
     * (should be called every loop cycle until it's done)
     * @returns True when it's done, false otherwise
     */
    bool ZeroElevator();

    /**
     * Updates all sensors which require an update. Must be called
     * each loop cycle, before using the driver.
     */
    void UpdateSensors();

    void SetDriverPower(double power);
    void SetElevatorPower(double power);
    void DriveStake();

    // Guarantee the singleton
    Driver(Driver const&) = delete;
    void operator=(Driver const&) = delete;

   private:
    ElevatorMotor mElevator{ElevatorConstants::kMotorPin, ElevatorConstants::kEncoderFwdPin, ElevatorConstants::kEncoderBckPin};
    HytorcSimple mActuator{DriverConstants::kHytorcMotorPin, DriverConstants::kHytorcEncoderForward, DriverConstants::kHytorcEncoderBackward};
    AccelStepper mHandoff{4, HandoffConstants::kStepPin1, HandoffConstants::kStepPin2, HandoffConstants::kStepPin3, HandoffConstants::kStepPin4};

    // RCInput rcInput;
    Logger& mLogger;

    LimitSwitch mFirstStake{HandoffConstants::kLimitSwitch1Pin};
    LimitSwitch mSecondStake{HandoffConstants::kLimitSwitch2Pin};
    LimitSwitch mThirdStake{HandoffConstants::kLimitSwitch3Pin};

    LimitSwitch mElevatorZero{ElevatorConstants::kTopLimitSwitchPin};    // top
    LimitSwitch mElevatorEnd{ElevatorConstants::kBottomLimitSwitchPin};  // bottom

    Thermistor mActuatorTemp{DriverConstants::kHytorcThermistorPin};
    CurrentSensor mActuatorCurrent{DriverConstants::kHytorcCurrentPin};
    CurrentSensor mElevatorCurrent{ElevatorConstants::kCurrentPin};

    PIDF mElevatorController{ElevatorConstants::kP, ElevatorConstants::kI, ElevatorConstants::kD,
                             GetElevatorFeedforward(ElevatorConstants::kS, ElevatorConstants::kV, ElevatorConstants::kA, ElevatorConstants::kG, 0, 0)};
    PIDF mActuatorController{DriverConstants::kP, DriverConstants::kI, DriverConstants::kD,
                             GetSimpleFeedforward(DriverConstants::kS, DriverConstants::kV, DriverConstants::kA, 0, 0)};

    bool mInvertHandoffOutput = false;
    bool mStakeLimitSwitchContact = false;
    double mPowerScaler = 1.0;
    int mHandoffDir = 1;
    LimitSwitch& mTargetLimitSwitch = mFirstStake;

    StakeNumber mStakeState = StakeNumber::kOne;

    // If the voltage for the elvator drops below this threshold, assume we're
    // stuck (ie: drawing a very high current)
    static constexpr double kElevatorMinVoltage = 0.01;
    // In milliseconds
    static const int kElevatorLoopDelay = 50;
    // Out of 1
    static constexpr double kElevatorZeroSpeed = 1;

    static constexpr double kHandoffSpeed = 600;

    /**
     * Creates the driver subsystem, using the pins in Util.h
     */
    Driver();

    bool RunHandoff(LimitSwitch& targetLimitSwitch);
};

}  // namespace lattice