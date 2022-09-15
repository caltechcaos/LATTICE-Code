#include <Arduino.h>

#include "ButtonOut.h"
#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Util.h"

lattice::RC controller(Serial3, 15, 53);
auto& atrv = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    atrv.Setup();
    controller.Setup();
    // Auto presses the button on startup
}

double deadband(double input) {
    if (abs(input) < 0.25) {
        return 0;
    } else {
        return input;
    }
}

void loop() {
    controller.Update();
    double ail = deadband((controller.GetAileron()));  // /2
    double elev = deadband((controller.GetElevator()));
    double throt = (double)(controller.GetThrottle());
    double rud = (double)(controller.GetRudder());
    double gear = (double)(controller.GetGear());
    double aux1 = (double)(controller.GetAux1());
    double aux2 = (double)(controller.GetAux2());

    // Serial.print("Hi");
    // x = lattice::remap(x, 0.0, 1.0, -.5, .5);
    // y = lattice::remap(y, 0.0, 1.0, -.5, .5);
    Serial.print(ail);
    Serial.print(", ");
    Serial.println(elev);
    // Serial.print(", ");
    // Serial.print(throt);
    // Serial.print(", ");
    // Serial.print(rud);
    // Serial.print(", ");
    // Serial.print(gear);
    // Serial.print(", ");
    // Serial.print(aux1);
    // Serial.print(", ");
    // Serial.println(aux2);
    atrv.Move(ail, elev);  // 0.5 + //0.5+
}
