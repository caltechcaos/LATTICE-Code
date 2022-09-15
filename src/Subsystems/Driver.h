#pragma once
#include "Clifford.h"
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "HandoffMotor.h"
#include "HytorcSimple.h"
#include "LimitSwitch.h"
#include "Logger.h"
#include "PIDF.h"
#include "Thermistor.h"
#include <Stepper.h>

namespace lattice {
class Driver {
   public:
    enum class StakeNumber {
        kOne,
        kTwo,
        kThree
    };

    enum class DriverState {
        kIdle,
        kZeroElevator,
        kHandoff,
        kAutoDrive,
        kManual
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

    void SetState(DriverState state);

    void SetDriverPower(double power);
    void SetElevatorPower(double power);
    void DriveStake();

    // Guarantee the singleton
    Driver(Driver const&) = delete;
    void operator=(Driver const&) = delete;

   private:
    ElevatorMotor mElevator;
    HytorcSimple mActuator;
    Stepper mHandoff;

    // RCInput rcInput;
    Logger& mLogger;

    LimitSwitch mFirstStake;
    LimitSwitch mSecondStake;
    LimitSwitch mThirdStake;

    LimitSwitch mElevatorZero;  // top
    LimitSwitch mElevatorEnd;   // bottom

    Thermistor mActuatorTemp;
    CurrentSensor mActuatorCurrent;
    CurrentSensor mElevatorCurrent;

    PIDF mElevatorController;
    PIDF mActuatorController;

    bool mInvertHandoffOutput = false;
    bool mStakeLimitSwitchContact = false;
    double mPowerScaler = 1.0;
    int mHandoffDir = 1;
    LimitSwitch& mTargetLimitSwitch = mFirstStake;

    DriverState mState;
    DriverState mPrevState;

    StakeNumber mStakeState = StakeNumber::kOne;

    // If the voltage for the elvator drops below this threshold, assume we're
    // stuck (ie: drawing a very high current)
    static constexpr double kElevatorMinVoltage = 0.01;
    // In milliseconds
    static const int kElevatorLoopDelay = 50;
    // Out of 1
    static constexpr double kElevatorZeroSpeed = 1;

    static constexpr double kHandoffSpeed = 10;

    /**
     * Creates the driver subsystem, using the pins in Util.h
     */
    Driver();



    /**
     * Runs the elevator controller one tick (using PIDF current control)
     * @param setpoint The target current, in amps
     * @returns True if the controller is done, false otherwise
     */
    bool RunElevatorOneTick(double setpoint);

    bool RunStakeHandoff(LimitSwitch& targetLimitSwitch);
};

}  // namespace lattice