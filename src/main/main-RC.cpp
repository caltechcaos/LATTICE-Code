// NOTE: Deprecated for splitting into Shuttle and Driver
#include <Arduino.h>

#include "RC.h"
#include "Subsystems/Clifford.h"
#include "Subsystems/Driver.h"
#include "Subsystems/Shuttle.h"
#include "Util.h"

#define RPMSCALE 5700

lattice::RC controller(Serial3, 15, 22);  // TODO: Use correct wiring when ATVR is fixed
// Runs at init
void setup() {
    lattice::GenericSetup();
    controller.Setup();
}

// Runs continously
void loop() {
    controller.Update();
    Serial.print(controller.GetGear());
    Serial.print(", ");
    Serial.print(controller.GetElevator());

    Serial.println();
}