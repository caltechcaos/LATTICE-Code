#pragma once
#include "Clifford.h"
#include "CurrentSensor.h"
#include "ElevatorMotor.h"
#include "HandoffMotor.h"
#include "LimitSwitch.h"
#include "PIDF.h"
#include "Thermistor.h"
#include "Logger.h"

namespace lattice {
class Driver {
   public:
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

    // Guarantee the singleton
    Driver(Driver const&) = delete;
    void operator=(Driver const&) = delete;

   private:
    ElevatorMotor elevator;
    // HytorcMotor actuator;
    HandoffMotor handoff;

    // RCInput rcInput;
    Logger &logger;

    LimitSwitch firstStake;
    LimitSwitch secondStake;
    LimitSwitch thirdStake;

    LimitSwitch elevatorZero;  // top
    LimitSwitch elevatorEnd;   // bottom

    Thermistor actuatorTemp;
    CurrentSensor actuatorCurrent;
    CurrentSensor elevatorCurrent;

    PIDF elevatorController;
    PIDF actuatorController;

    enum class State {
        Idle,
        ZeroElevator
    };

    State state;

    // If the voltage for the elvator drops below this threshold, assume we're
    // stuck (ie: drawing a very high current)
    static constexpr double kElevatorMinVoltage = 0.01;
    // In milliseconds
    static const int kElevatorLoopDelay = 50;
    // Out of 1
    static constexpr double kElevatorZeroSpeed = 1;

    /**
     * Creates the driver subsystem, using the pins in Util.h
     */
    Driver();

    /**
     * Updates all sensors which require an update. Must be called
     * each loop cycle, before using the driver.
     */
    void UpdateSensors();

    /**
     * Runs the elevator controller one tick (using PIDF current control)
     * @param setpoint The target current, in amps
     * @returns True if the controller is done, false otherwise
     */
    bool RunElevatorOneTick(double setpoint);
};

}  // namespace lattice