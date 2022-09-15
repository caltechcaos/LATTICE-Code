#pragma once
#include <Arduino.h>

namespace lattice {

class ATRVMotor {
   public:
    ATRVMotor(int forwardPWMPin, int backwardPWMPin);
    void Setup();
    void Enable();
    void Disable();

    void SetPWM(int forwardPWM, int backwardPWM);
    void SetPower(double power);

   private:
    const int kForwardPWMPin;
    const int kBackwardPWMPin;
};
}  // namespace lattice
