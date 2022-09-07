#include <Arduino.h>

#include "LimitSwitch.h"

lattice::LimitSwitch limitswitch{53};
void setup() {
    Serial.begin(115200);
    limitswitch.Setup();
}

void loop() {
    limitswitch.Update();
    Serial.println(limitswitch.Get());
}