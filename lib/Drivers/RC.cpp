#include "RC.h"

#include <Arduino.h>

using namespace lattice;
RC::RC(UARTClass& comSerial, int rxPin, int powerPin) : mSatellite(comSerial), mComSerial(comSerial), kRxPin(rxPin), kPowerPin(powerPin) {}

void RC::Setup() {
    mSatellite.setBindingMode(Internal_DSMx_22ms);
    mSatellite.startBinding(kPowerPin, kRxPin);
    mPrevBlink = millis();
    mComSerial.begin(115200);
}

double RC::ConvertOutput(double controllerInput) {
    double rawPercent = (controllerInput - kMinThrottle) / (kMaxThrottle - kMinThrottle);
    rawPercent = rawPercent * 2 - 1;
    if (abs(rawPercent) < kDeadband) {
        rawPercent = 0;
    }
    return rawPercent;
}

void RC::Update() {
    if (!mSatellite.getFrame()) {
        // mBinded = false;
        auto currTime = millis();
        if (currTime - mPrevBlink >= kLEDBlink) {
            mLEDState != mLEDState;
            mPrevBlink = currTime;
        }
    } else {
        mBinded = true;
    }
}

double RC::GetThrottle() {
    if (mBinded) {
        return ConvertOutput(mSatellite.getThrottle());
    } else {
        return 0.0;
    }
}

int RC::ProcessMinMidMaxInput(u_int16_t value) {
    if (mBinded) {
        // switch (value) {
        //     case kMinThrottle:
        //         return 0;
        //         break;
        //     case kMidThrottle:
        //         return 1;
        //         break;
        //     case kMaxThrottle:
        //         return 2;
        //         break;
        //}
        return value;
    } else {
        return 0;
    }
}

double RC::GetAileron() {
    if (mBinded) {
        return ConvertOutput(mSatellite.getAileron());
    } else {
        return 0.0;
    }
}
double RC::GetElevator() {
    if (mBinded) {
        return ConvertOutput(mSatellite.getElevator());
    } else {
        return 0.0;
    }
}
double RC::GetRudder() {
    if (mBinded) {
        return ConvertOutput(mSatellite.getRudder());
    } else {
        return 0.0;
    }
}
int RC::GetGear() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getGear() - kMinThrottle) / (kMaxThrottle - kMinThrottle);

        if (rawPercent < 0.3) {
            return 2;
        } else if (rawPercent > 0.6) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
int RC::GetAux1() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getAux1() - kMinThrottle) / (kMaxThrottle - kMinThrottle);

        if (rawPercent < 0.3) {
            return 2;
        } else if (rawPercent > 0.6) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
int RC::GetAux2() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getAux2() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        Serial.println(rawPercent);
        if (rawPercent < 0.3) {
            return 1;
        } else {
            return 0;
        }
    } else {
        // By default return 1 which is ESTOP
        return 1;
    }
}
