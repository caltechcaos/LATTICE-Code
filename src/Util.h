#pragma once

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice {
constexpr int SERIAL_RATE = 115200;  // baud
constexpr int ANALOG_BITS = 12;      // resolution

/**
 * Sets all parameters. Every main should call this.
 */
void GenericSetup();

namespace CliffordConstants {
constexpr int kJoystickTriggerPin = 0;
}

namespace ElevatorConstants {
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
}  // namespace ElevatorConstants

namespace HandoffConstants {
constexpr int kMotorPin1 = 0;
constexpr int kMotorPin2 = 0;
constexpr int kMotorPin3 = 0;
constexpr int kMotorPin4 = 0;

constexpr int kLimitSwitch1Pin = 0;
constexpr int kLimitSwitch2Pin = 0;
constexpr int kLimitSwitch3Pin = 0;

}  // namespace HandoffConstants

namespace DriverConstants {

constexpr int kHytorcThermistorPin = 0;
constexpr int kHytorcCurrentPin = 0;

constexpr double kP = 0;
constexpr double kI = 0;
constexpr double kD = 0;

constexpr double kS = 0;
constexpr double kV = 0;
constexpr double kA = 0;

}  // namespace DriverConstants

}  // namespace lattice