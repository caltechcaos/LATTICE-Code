#include "LimitSwitch.h"

#include <Arduino.h>

using namespace lattice;

LimitSwitch::LimitSwitch(const int pin) : mPin(pin), mState(false), mLast(false) {}

void LimitSwitch::Setup() {
    pinMode(mPin, INPUT);
    digitalWrite(mPin, HIGH);
}
void LimitSwitch::Update() {
    mLast = mState;
    mState = digitalRead(mPin) == LOW;
}

bool LimitSwitch::Get() {
    return mState;
}

bool LimitSwitch::Pushed() {
    return mState && !mLast;
}

bool LimitSwitch::Released() {
    return !mState && mLast;
}