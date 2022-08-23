#pragma once

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice {
    const int SERIAL_RATE = 115200; // baud
    const int ANALOG_BITS = 12; // resolution

    /**
     * Sets all parameters. Every main should call this.
     */
    void GenericSetup();

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
}