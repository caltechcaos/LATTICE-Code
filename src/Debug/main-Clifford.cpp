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
    // pinMode (CS, OUTPUT); //CS - When High, sets chip to read the data.
    // pinMode (SHDN, OUTPUT); //CS - When High, sets chip to read the data.
    // pinMode (RS, OUTPUT); //CS - When High, sets chip to read the data.
  
    // digitalWrite(SHDN, HIGH); //Power ON (HIGH)
    // digitalWrite(RS, HIGH); //Power NO RESET (LOW)
    // SPI.begin();
}

void loop() {
    controller.Update();
    double x = (double)(controller.GetAileron() ); // /2
    double y = (controller.GetThrottle());
    //Serial.print("Hi");
    //x = lattice::remap(x, 0.0, 1.0, -.5, .5);
    //y = lattice::remap(y, 0.0, 1.0, -.5, .5);
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
    atrv.Move(x, y); //0.5 + //0.5+
}
