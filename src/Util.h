#pragma once

#include <Arduino.h>
#undef min
#undef max

#include <string>

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice
{
    constexpr int SERIAL_RATE = 115200; // baud
    constexpr int ANALOG_BITS = 12;     // resolution

    /**
     * remaps values in this order:
     * long x, long in_min, long in_max, long out_min, long out_max
     */
    double remap(double x, double in_min, double in_max, double out_min, double out_max);

    /**
     * Sets all parameters. Every main should call this.
     */
    void GenericSetup(std::string name = "");

    namespace CliffordConstants
    {
        constexpr int kJoystickTriggerPin = 22;
    }

    namespace ElevatorConstants
    {
        constexpr int kMotorPin = 0;
        constexpr int kEncoderFwdPin = 0;
        constexpr int kEncoderBckPin = 0;

        constexpr int kTopLimitSwitchPin = 0;
        constexpr int kBottomLimitSwitchPin = 0;

        constexpr int kCurrentPin = 0;

        constexpr double kP = 0;
        constexpr double kI = 0;
        constexpr double kD = 0;

        constexpr double kS = 0;
        constexpr double kV = 0;
        constexpr double kA = 0;
        constexpr double kG = 0;
    } // namespace ElevatorConstants

    namespace HandoffConstants
    {
        // constexpr int kMotorPin1 = 0;
        // constexpr int kMotorPin2 = 0;
        // constexpr int kMotorPin3 = 0;
        // constexpr int kMotorPin4 = 0;

        constexpr int kLimitSwitch1Pin = 0;
        constexpr int kLimitSwitch2Pin = 0;
        constexpr int kLimitSwitch3Pin = 0;

        //hand off mech stepper motor 
        constexpr int kStepPin1 = 8;
        constexpr int kStepPin2 = 9;
        constexpr int kStepPin3 = 10;
        constexpr int kStepPin4 = 11;
        //constexpr int kDrivePin = 3;
        constexpr int kStepsPerRev = 200;
        constexpr int kRPM = 120;
        constexpr int kMotorAccel = 2000;
        constexpr int kMotorDecel = 1000;
        constexpr int kMicrosteps = 16;
        //constexpr int kSleepPin = 13;

    } // namespace HandoffConstants

    namespace DriverConstants
    {
        constexpr int kHytorcMotorPin = 0;
        constexpr int kHytorcEncoderForward = 0;
        constexpr int kHytorcEncoderBackward = 0;
        constexpr int kHytorcThermistorPin = 0;
        constexpr int kHytorcCurrentPin = 0;

        constexpr double kP = 0;
        constexpr double kI = 0;
        constexpr double kD = 0;

        constexpr double kS = 0;
        constexpr double kV = 0;
        constexpr double kA = 0;

    } // namespace DriverConstants

    namespace ShuttleConstants
    {
        constexpr int kBrakePin = A4;
        // This have to be analogPin and cannot exceed 5V
        constexpr int kVoltagePin = A0;
        constexpr double kMaxInputVoltage = 1.2; // temp (dependson fully charge bat and resistor)
        constexpr double kMultiplier = 3.6;      // temp (depends on how much of the origional voltage is inputed)(I.E. 1/3)

        constexpr int kLeftMotionMotorEnablePin = 0;
        constexpr int kRightMotionMotorEnablePin = 0;
        constexpr int kLeftMotionMotorSignalPin = 0;
        constexpr int kRightMotionMotorSignalPin = 0;

        constexpr int kLeftMotionMotorRPMPin = 0;
        constexpr int kRightMotionMotorRPMPin = 0;

        constexpr int kLeftMotionMotorThermalPin = 0;
        constexpr int kRightMotionMotorThermalPin = 0;

        constexpr int kLeftHytorcMotorPin = 0;
        constexpr int kRightHytorcMotorPin = 0;

        constexpr int kLeftHytorcForwardEncoderPin = 0;
        constexpr int kRightHytorcForwardEncoderPin = 0;
        constexpr int kLeftHytorcBackwardEncoderPin = 0;
        constexpr int kRightHytorcBackwardEncoderPin = 0;

        constexpr int kLeftArmLimitSwitchPin = 0;
        constexpr int kRightArmLimitSwitchPin = 0;
        constexpr int kLeftLimitSwitchPin = 0;
        constexpr int kCenterLimitSwitchPin = 0;
        constexpr int kRightLimitSwitchPin = 0;

        constexpr double kP = 0;
        constexpr double kI = 0;
        constexpr double kD = 0;
    } // namespace ShuttleConstants

} // namespace lattice