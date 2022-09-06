#include "Digipot.h"

#include <Arduino.h>
#include <SPI.h>

using namespace lattice;

// cf. https://wiki.dfrobot.com/Dual_Digital_Pot__100K__SKU__DFR0520

Digipot::Digipot() {}

void Digipot::Setup() {
    // Required for the SS pin
    pinMode(SS_PIN, OUTPUT);

    SPI.begin();

    // If they're not zeroed to begin with, the resistance readings aren't consistent
    // between trials
    SetBoth(0);
}

bool Digipot::Set1(int resistance) {
    return SetPot(POT1, resistance);
}

bool Digipot::Set2(int resistance) {
    return SetPot(POT2, resistance);
}

bool Digipot::SetBoth(int resistance) {
    return SetPot(POT_BOTH, resistance);
}

bool Digipot::SetPot(const int POT, int resistance) {
    bool success = true;
    if (resistance < 0) {
        resistance = 0;
        success = false;
    } else if (resistance > DIGIPOT_MAX) {
        resistance = DIGIPOT_MAX;
        success = false;
    }

    digitalWrite(SS_PIN, LOW);
    SPI.transfer(POT);
    SPI.transfer(resistance);
    digitalWrite(SS_PIN, HIGH);
    return success;
}