#include <Arduino.h>

#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Util.h"
#include "ButtonOut.h"


lattice::RC controller(Serial1, 19, 2, 13);
lattice::ButtonOut trigger(lattice::CliffordConstants::kJoystickTriggerPin);
auto& atrv = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    atrv.Setup();
    controller.Setup();
    //Auto presses the button on startup
    trigger.Setup();
}

void loop() {
    controller.Update();
    double ail = (double)(controller.GetAileron() ); // /2
    double elev = (controller.GetElevator());
    double throt = (double) (controller.GetThrottle());
    double rud = (double) (controller.GetRudder());
    double gear = (double) (controller.GetGear());
    double aux1 = (double) (controller.GetAux1());
    double aux2 = (double) (controller.GetAux2());

    //Serial.print("Hi");
    //x = lattice::remap(x, 0.0, 1.0, -.5, .5);
    //y = lattice::remap(y, 0.0, 1.0, -.5, .5);
    Serial.print(ail);
    Serial.print(", ");
    Serial.println(elev);
    //Serial.print(", ");
    //Serial.print(throt);
    //Serial.print(", ");
    //Serial.print(rud);
    //Serial.print(", ");
    //Serial.print(gear);
    //Serial.print(", ");
    //Serial.print(aux1);
    //Serial.print(", ");
    //Serial.println(aux2);
    atrv.Move(x, y); //0.5 + //0.5+
}
