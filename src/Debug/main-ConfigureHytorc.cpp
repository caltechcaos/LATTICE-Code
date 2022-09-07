#include "HytorcSimple.h"
#include "Util.h"
lattice::HytorcSimple mMotor{32, 24, 25};
void setup() {
    lattice::GenericSetup();
    mMotor.Setup();
}
double power = 0;
void loop() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();
        if (input == 'a') {
            power = 1;
        } else if (input == 'b') {
            power = -1;
        } else if (input == 'e') {
            power = 0;
        }
    }
    mMotor.SetPercentOutput(power);
}