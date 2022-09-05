#include <Arduino.h>

#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Util.h"

lattice::RC controller(Serial1, Serial, 19, 2, 13);
auto& atrv = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    atrv.Setup();
    controller.Setup();
}

void loop() {
    controller.Update();
    atrv.Move((double)(controller.GetRudder()) / 2, controller.GetThrottle());
}