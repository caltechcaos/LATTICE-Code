#include <Arduino.h>

#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Util.h"

lattice::RC controller(Serial1, Serial, 19, 2, 13);
auto& atrv = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    atrv.Setup();
}

void loop() {
    atrv.Move((double)(controller.GetRudder() - 342) / (1700 - 342), controller.GetThrottle());
}