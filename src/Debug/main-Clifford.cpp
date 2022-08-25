#include <Arduino.h>

#include "Subsystems/Clifford.h"
#include "Util.h"

void setup() {
    lattice::GenericSetup();
    lattice::Clifford::clifford().Setup();
}

void loop () {
    // clifford().Move(x, y);
}