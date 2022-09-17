#include "Clifford.h"

#include <Arduino.h>

#include "Util.h"

using namespace lattice;

Clifford::Clifford() {}

void Clifford::Setup() {
    Zero();
    mLeftMotor.Setup();
    mRightMotor.Setup();
}

bool Clifford::SetBrake(bool brake) {
    if (brake == Clifford::brake) {
        return false;
    }
    Clifford::brake = brake;
    return true;
}

void Clifford::Zero() {
    Move(0, 0);
}

bool Clifford::Move(double forward, double rot) {
    if (!brake) {
        double left = forward + rot;
        double right = forward - rot;
        mLeftMotor.SetPower(kScaler * left);
        mRightMotor.SetPower(-kScaler * right);
    } else {
        mLeftMotor.SetPower(0);
        mRightMotor.SetPower(0);
        return false;
    }

    return true;
}