#include "RC.h"

#include <Arduino.h>

using namespace lattice;
RC::RC(UARTClass& comSerial, int rxPin, int powerPin, int onboardLEDPin) : mSatellite(comSerial), mComSerial(comSerial), kRxPin(rxPin), kPowerPin(powerPin), kOnboardLEDPin(onboardLEDPin) {}

void RC::Setup() {
    pinMode(kOnboardLEDPin, OUTPUT);
    digitalWrite(kOnboardLEDPin, HIGH);

    mSatellite.setBindingMode(Internal_DSMx_22ms);
    mSatellite.startBinding(kPowerPin, kRxPin);
    mPrevBlink = millis();
    mComSerial.begin(115200);
}

void RC::Update() {
    if (!mSatellite.getFrame()) {
        //mBinded = false;
        auto currTime = millis();
        if (currTime - mPrevBlink >= kLEDBlink) {
            mLEDState != mLEDState;
            digitalWrite(kOnboardLEDPin, mLEDState);
            mPrevBlink = currTime;
        }
    } else {
        mBinded = true;
        digitalWrite(kOnboardLEDPin, HIGH);
    }
}

double RC::GetThrottle() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getThrottle() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        return rawPercent;
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
        double rawPercent = (double)(mSatellite.getAileron() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        return rawPercent;
    } else {
        return 0.0;
    }
}
double RC::GetElevator() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getElevator() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        return rawPercent;
    } else {
        return 0.0;
    }
}
double RC::GetRudder() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getRudder() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        return rawPercent;
    } else {
        return 0.0;
    }
}
int RC::GetGear() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getGear() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        
        if (rawPercent<0.3)
        {
            return 2;
        }
        else if (rawPercent>0.6)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    } else {
        return 0;
    }
}
int RC::GetAux1() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getAux1() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
       
       if (rawPercent<0.3)
        {
            return 2;
        }
        else if (rawPercent>0.6)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    } else {
        return 1;
    }
}
int RC::GetAux2() {
    if (mBinded) {
        double rawPercent = (double)(mSatellite.getAux2() - kMinThrottle) / (kMaxThrottle - kMinThrottle);
        if (rawPercent<0.3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    } else {
        return 0.0;
    }
}
