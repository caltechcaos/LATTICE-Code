#include <Arduino.h>

#include "Digipot.h"
#include "Util.h"

lattice::Digipot digipot;
int val;
int step;

void setup() {
    lattice::GenericSetup();
    digipot.Setup();
    val = 10;
    step = 1;
}

void loop() {
    digipot.Set1(val);
    Serial.println(val);
    val += step;
    if (val >= lattice::Digipot::DIGIPOT_MAX || val <= 0) {
        step *= -1;
    }
    delay(5000);
}