#include "ElevatorMotor.h"

using namespace lattice;

// cf. https://docs.revrobotics.com/sparkmax/operating-modes/control-interfaces#pwm-interface

ElevatorMotor::ElevatorMotor(const int kElevatorPin, bool invert)
    : motor(), kPin(kElevatorPin), brake(false), invert(invert) {}

void ElevatorMotor::Setup() {
    motor.attach(kPin);
    Run(0);  // Stop it
}

bool ElevatorMotor::Run(double power) {
    if (brake) {
        return false;
    }
    bool success = true;
    if (power < -1) {
        power = -1;
        success = false;
    } else if (power > 1) {
        power = 1;
        success = false;
    }

    // Map from [-1, 1] to [1000, 2000] microseconds
    motor.writeMicroseconds((invert ? -1 : 1) * power * 500 + 1500);
    return success;
}

bool ElevatorMotor::SetBrake(bool brake) {
    if (brake == ElevatorMotor::brake) {
        return false;
    }

    if (brake) {
        Run(0);
    }
    ElevatorMotor::brake = brake;
    return true;
}

void ElevatorMotor::Zero() {
}

double ElevatorMotor::GetPosition() {
    return 0.0;
}

void ElevatorMotor::SetVoltage(double desiredVoltage, double batteryVoltage) {
    Run(desiredVoltage / batteryVoltage);
}