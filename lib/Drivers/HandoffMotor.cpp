#include "HandoffMotor.h"
#include "Util.h"

using namespace lattice;

HandoffMotor::HandoffMotor(const int StepperPin, const int DriverPin, const int StepPerRev): mMotor(StepPerRev, DriverPin, StepperPin){ }
            //, const int kRPM, const int kMicrosteps //, MicroSteps(kMicrosteps)

void HandoffMotor::Setup(){
    mMotor.enable();
    mMotor.begin(RPM, MicroSteps);
    //mMotor.setMicrostep(1);  // Set microstep mode to 1:1
}

// void HandoffMotor::SetMicroStep(int mode){
//     Mode = mode;
//     mMotor.setMicrostep(mode);   // Set microstep mode 
// }

// void HandoffMotor::SetAccel(int MotorAccel, int MotorDecel) {
//     mMotor.setSpeedProfile(mMotor.LINEAR_SPEED, MotorAccel, MotorDecel);
//     bool success = speed >= 0;
//     if (!success) {
//         speed = 0;
//     }
//     //mMotor.setSpeed(speed);
//     return success;
// 
// }

void HandoffMotor::SetSpeed(int RPM){
    mMotor.setRPM(RPM);
}

void HandoffMotor::StepMove(int steps){
    mMotor.move(steps);
}

// void HandoffMotor::MicroStepMove(int steps){
//     mMotor.move(Mode * steps);
// }



// bool HandoffMotor::Move(int steps) {
//     if (brake) {
//         return false;
//     }
//     //mMotor.step(steps);
//     return true;
// }

bool HandoffMotor::SetBrake(bool brake) {
    if (brake == HandoffMotor::brake) {
        return false;
    }

    HandoffMotor::brake = brake;
    return true;
}