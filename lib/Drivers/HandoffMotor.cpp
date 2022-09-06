#include "HandoffMotor.h"

using namespace lattice;

HandoffMotor::HandoffMotor(const int pin1, const int pin2,
            const int pin3, const int pin4) : mMotor(STEPS_PER_REV, pin1, pin2, pin3, pin4) { }

bool HandoffMotor::SetSpeed(long speed) {
    bool success = speed >= 0;
    if (!success) {
        speed = 0;
    }
    mMotor.setSpeed(speed);
    return success;
}

bool HandoffMotor::Move(int steps) {
    if (brake) {
        return false;
    }
    mMotor.step(steps);
    return true;
}

bool HandoffMotor::SetBrake(bool brake) {
    if (brake == HandoffMotor::brake) {
        return false;
    }

    HandoffMotor::brake = brake;
    return true;
}