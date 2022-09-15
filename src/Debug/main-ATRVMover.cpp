#include <Arduino.h>

#include "ATRVMotor.h"
#include "Subsystems/Clifford.h"
#include "Util.h"

auto& clifford = lattice::Clifford::clifford();
void setup() {
    lattice::GenericSetup();
    clifford.Setup();
}
double forward = 0;
double rot = 0;
bool enable = false;
void loop() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            forward = 0.5;
        } else if (input == 'b') {
            rot = 0.5;
        } else if (input == 'c') {
            forward = -0.5;
        } else if (input == 'd') {
            rot = -0.5;
        } else if (input == 'e') {
            rot = 0;
        } else if (input == 'f') {
            forward = 0;
        } else if (input == 'q') {
            forward = 0;
            rot = 0;
        }
    }
    clifford.Move(forward, rot);
    Serial.print(forward);
    Serial.print(", ");
    Serial.println(rot);
}