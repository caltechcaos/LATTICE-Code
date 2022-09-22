#include "ATRVMotor.h"

#include <Arduino.h>
using namespace lattice;

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