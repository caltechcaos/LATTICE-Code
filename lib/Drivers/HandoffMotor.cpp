#include "HandoffMotor.h"

using namespace lattice;

HandoffMotor::HandoffMotor(const int steps, const int pin1, const int pin2,
            const int pin3, const int pin4) : mMotor(steps, pin1, pin2, pin3, pin4) { }

bool HandoffMotor::SetSpeed(long speed) {
    if (speed < 0) {
        return false;
    }
    mMotor.setSpeed(speed);
    return true;
}

void HandoffMotor::Move(int steps) {
    mMotor.step(steps);
}