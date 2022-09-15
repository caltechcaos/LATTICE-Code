#include "ATRVMotor.h"

#include <Arduino.h>

//#include "BTS7960.h"

using namespace lattice;

// ATRVMotor::ATRVMotor(const int LeftPin, const int RightPin):LeftM(LeftPin), RightM(RightPin) { }

// ATRVMotor::ATRVMotor(uint8_t F_PWM, uint8_t B_PWM) : ATRVMotor(F_PWM, B_PWM) {}
// EN?
ATRVMotor::ATRVMotor(int forwardPWMPin, int backwardPWMPin) : kForwardPWMPin(forwardPWMPin), kBackwardPWMPin(backwardPWMPin) {}

void ATRVMotor::Setup() {
    pinMode(kForwardPWMPin, OUTPUT);
    pinMode(kBackwardPWMPin, OUTPUT);
}

void ATRVMotor::SetPWM(int forwardPWM, int backwardPWM) {
    analogWrite(kForwardPWMPin, forwardPWM);
    analogWrite(kBackwardPWMPin, backwardPWM);
}

void ATRVMotor::SetPower(double power) {
    if (power > 1) {
        power = 1;
    } else if (power < -1) {
        power = -1;
    }
    int outpower = abs(power) * 4095;
    if (power > 0) {
        SetPWM(outpower, 0);
    } else if (power < 0) {
        SetPWM(0, outpower);
    } else {
        SetPWM(0, 0);
    }
}