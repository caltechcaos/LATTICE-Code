#include <Arduino.h>
#include "CTREEncoder.h"
#include "Util.h"

lattice::CTREEncoder m_encoder{26, 27};
void setup() {
    // put your setup code here, to run once:
    lattice::GenericSetup();
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println(m_encoder.GetEncoderRotation());
}