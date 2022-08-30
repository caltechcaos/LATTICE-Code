#pragma once

#undef min
#undef max
#include <string>

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice {
const int SERIAL_RATE = 115200;  // baud
const int ANALOG_BITS = 12;      // resolution

/**
 * Sets all parameters. Every main should call this.
 */
void GenericSetup(std::string name = "");

// PINS
const int kJoystickTriggerPin = 0;

const int kElevatorMotorPin = 0;
const int kElevatorEncoderFwdPin = 0;
const int kElevatorEncoderBckPin = 0;

const int kHandoffMotorPin1 = 0;
const int kHandoffMotorPin2 = 0;
const int kHandoffMotorPin3 = 0;
const int kHandoffMotorPin4 = 0;

const int kHandoffLimitSwitch1Pin = 0;
const int kHandoffLimitSwitch2Pin = 0;
const int kHandoffLimitSwitch3Pin = 0;

const int kElevatorTopLimitSwitchPin = 0;
const int kElevatorBottomLimitSwitchPin = 0;

const int kDriverHytorcThermistorPin = 0;
const int kDriverHytorcCurrentPin = 0;
const int kElevatorCurrentPin = 0;

// PIDF
constexpr double kPElevator = 0;
constexpr double kIElevator = 0;
constexpr double kDElevator = 0;

constexpr double kSElevator = 0;
constexpr double kVElevator = 0;
constexpr double kAElevator = 0;
constexpr double kGElevator = 0;

constexpr double kPDriver = 0;
constexpr double kIDriver = 0;
constexpr double kDDriver = 0;

constexpr double kSDriver = 0;
constexpr double kVDriver = 0;
constexpr double kADriver = 0;
}  // namespace lattice