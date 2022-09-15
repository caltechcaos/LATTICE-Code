#include "StepperHandOff.h"

using namespace lattice;

StepperHandOff::StepperHandOff(const int NumPin, const int pin1, const int pin2,
            const int pin3, const int pin4) : mMotor(NumPin, pin1, pin2, pin3, pin4) { }

void StepperHandOff::Setup(int MaxSpeed){
    mMotor.setMaxSpeed(MaxSpeed);
}

// void StepperHandOff::run(){
//     mMotor.runSpeed();
// }

bool StepperHandOff::SetSpeed(long speed) {
    bool success = speed >= 0;
    if (!success) {
        speed = 0;
    }
    mMotor.setSpeed(speed);
    mMotor.runSpeed();
    return success;
}

bool StepperHandOff::Move(int steps) {
    if (brake) {
        return false;
    }
    mMotor.move(steps);
    return true;
}

bool StepperHandOff::SetBrake(bool brake) {
    if (brake == StepperHandOff::brake) {
        return false;
    }

    StepperHandOff::brake = brake;
    return true;
}
