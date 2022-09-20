#include "HallEffect.h"
#include "Util.h"
lattice::HallEffect mHall{4};

void setup() {
    lattice::GenericSetup();
    mHall.Setup();
}

void loop() {
    Serial.println(mHall.Get());
}