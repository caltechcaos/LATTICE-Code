#include "RC.h"

#include <Arduino.h>

using namespace lattice;
RC::RC(UARTClass comSerial, UARTClass logSerial, int rxPin, int powerPin, int onboardLEDPin) : mSatellite(comSerial), mComSerial(comSerial), mLogSerial(logSerial), kRxPin(rxPin), kPowerPin(powerPin), kOnboardLEDPin(onboardLEDPin) {}

void RC::Setup() {
    mSatellite.setLog(mLogSerial);
    pinMode(kOnboardLEDPin, OUTPUT);
    digitalWrite(kOnboardLEDPin, HIGH);

    mSatellite.setBindingMode(Internal_DSMx_22ms);
    mSatellite.startBinding(kPowerPin, kRxPin);
    mPrevBlink = millis();
    mComSerial.begin(115200);
}

void RC::Update() {
    if (!mSatellite.getFrame()) {
        auto currTime = millis();
        if (currTime - mPrevBlink >= kLEDBlink) {
            mLEDState != mLEDState;
            digitalWrite(kOnboardLEDPin, mLEDState);
            mPrevBlink = currTime;
        }
    } else {
        digitalWrite(kOnboardLEDPin, HIGH);
    }
}

double RC::GetThrottle() {
    double rawPercent = (double)(mSatellite.getThrottle() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
    return rawPercent;
}

int RC::ProcessMinMidMaxInput(u_int16_t value) {
    switch (value) {
        case kMinThrottle:
            return 0;
            break;
        case kMidThrottle:
            return 1;
            break;
        case kMaxThrottle:
            return 2;
            break;
    }
}

int RC::GetAileron() {
    return ProcessMinMidMaxInput(mSatellite.getAileron());
}
int RC::GetElevator() {
    return ProcessMinMidMaxInput(mSatellite.getElevator());
}
int RC::GetRudder() {
    return ProcessMinMidMaxInput(mSatellite.getRudder());
}
int RC::GetGear() {
    return mSatellite.getGear();
}
int RC::GetAux1() {
    return mSatellite.getAux1();
}
int RC::GetAux2() {
    return mSatellite.getAux2();
}
