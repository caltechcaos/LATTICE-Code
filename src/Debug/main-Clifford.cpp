#include <Arduino.h>

#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Util.h"

lattice::RC controller(Serial1, 19, 2, 13);
auto& atrv = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    atrv.Setup();
    controller.Setup();
}

void loop() {
    controller.Update();
    double x = (double)(controller.GetRudder()); // /2
    double y = controller.GetThrottle();
    //Serial.print("Hi");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
    atrv.Move(x, y);
}