#pragma once

#include <Arduino.h>
#undef min
#undef max

#include <string>

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice {
constexpr int SERIAL_RATE = 115200;  // baud
constexpr int ANALOG_BITS = 12;      // resolution

/**
 * remaps values in this order:
 * long x, long in_min, long in_max, long out_min, long out_max
 */
double remap(double x, double in_min, double in_max, double out_min, double out_max);

/**
 * Sets all parameters. Every main should call this.
 */
void GenericSetup(std::string name = "");

namespace CliffordConstants {
constexpr int kLeftMotorForwardPin = 1;
constexpr int kLeftMotorBackwardPin = 3;
constexpr int kRightMotorForwardPin = 12;
constexpr int kRightMotorBackwardPin = 13;
}  // namespace CliffordConstants

namespace ElevatorConstants {
constexpr int kMotorPin = 4;
constexpr int kEncoderFwdPin = 6;
constexpr int kEncoderBckPin = 7;

constexpr int kTopLimitSwitchPin = 10;
constexpr int kBottomLimitSwitchPin = 5;

constexpr int kCurrentPin = 53;

constexpr double kP = 0;
constexpr double kI = 0;
constexpr double kD = 0;

constexpr double kS = 0;
constexpr double kV = 0;
constexpr double kA = 0;
constexpr double kG = 0;

constexpr double kElevatorVoltage = 0.8;  // lattice::GetElevatorFeedforward(lattice::ElevatorConstants::kS, lattice::ElevatorConstants::kV, lattice::ElevatorConstants::kA, lattice::ElevatorConstants::kG, 0, f_desired);
}  // namespace ElevatorConstants

namespace HandoffConstants {
constexpr int kLimitSwitch1Pin = 24;
constexpr int kLimitSwitch2Pin = 22;
constexpr int kLimitSwitch3Pin = 26;

// hand off mech stepper motor
constexpr int kStepDirPin = 4;
constexpr int kStepPulPin = 2;
// constexpr int kSleepPin = 13;

}  // namespace HandoffConstants

namespace DriverConstants {
constexpr int kVoltageSensorPin = A0;
constexpr int kHytorcMotorPin = 52;
constexpr int kHytorcEncoderForward = 10;
constexpr int kHytorcEncoderBackward = 11;
constexpr int kHytorcThermistorPin = 12;
constexpr int kHytorcCurrentPin = 13;

constexpr double kP = 0;
constexpr double kI = 0;
constexpr double kD = 0;

constexpr double kS = 0;
constexpr double kV = 0;
constexpr double kA = 0;

constexpr double kDriverVoltage = 18;  // lattice::GetSimpleFeedforward(kS, kV, kA, 0, t_desired / I_s);

}  // namespace DriverConstants

namespace ShuttleConstants {
// This have to be analogPin and cannot exceed 5V
constexpr int kBatteryVoltagePin = A0;
constexpr double kMaxInputVoltage = 18.0;  // temp (dependson fully charge bat and resistor)
constexpr double kMultiplier = 1;          // temp (depends on how much of the origional voltage is inputed)(I.E. 1/3)

constexpr int kOuterRightMotionMotorEnablePin = 30;
constexpr int kInnerRightMotionMotorEnablePin = 28;

constexpr int kOuterLeftMotionMotorEnablePin = 26;
constexpr int kInnerLeftMotionMotorEnablePin = 24;

constexpr int kOuterLeftMotionMotorSignalPin = 5;
constexpr int kInnerLeftMotionMotorSignalPin = 4;

constexpr int kOuterRightMotionMotorSignalPin = 3;
constexpr int kInnerRightMotionMotorSignalPin = 2;

constexpr int kOuterLeftMotionMotorRPMPin = A8;
constexpr int kOuterRightMotionMotorRPMPin = A6;
constexpr int kInnerLeftMotionMotorRPMPin = A7;
constexpr int kInnerRightMotionMotorRPMPin = A5;

constexpr int kOuterLeftMotionMotorThermalPin = 9;
constexpr int kOuterRightMotionMotorThermalPin = 10;
constexpr int kInnerLeftMotionMotorThermalPin = 11;
constexpr int kInnerRightMotionMotorThermalPin = 12;

constexpr int kLeftHytorcMotorPin = 7;
constexpr int kRightHytorcMotorPin = 8;

constexpr int kLeftHytorcForwardEncoderPin = 37;
constexpr int kLeftHytorcBackwardEncoderPin = 38;

constexpr int kRightHytorcForwardEncoderPin = 39;

constexpr int kRightHytorcBackwardEncoderPin = 40;

constexpr int kLeftArmBottomLimitSwitchPin = 41;
// constexpr int kLeftArmTopLimitSwitchPin = 52;
constexpr int kRightArmBottomLimitSwitchPin = 42;
// constexpr int kRightArmTopLimitSwitchPin = 53;
constexpr int kCenterHallSwitchPin = 55;

constexpr int kArmBrakePin = 32;

constexpr double kTensionRPM = 700;

constexpr double kP = 1.0;
constexpr double kI = 0;
constexpr double kD = 0.5;
constexpr double kS = 1.8;
constexpr double kError = 0.5;
constexpr double kVError = 1.5;
}  // namespace ShuttleConstants

namespace RCPorts {
constexpr int kDriverRXPort = 15;
constexpr int kDriverPowerPort = 22;

constexpr int kShuttleRXPort = 15;
constexpr int kShuttlePowerPort = 22;
}  // namespace RCPorts
}  // namespace lattice