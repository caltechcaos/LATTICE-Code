#include <Arduino.h>
#include "CTREEncoder.h"

lattice::CTREEncoder m_encoder{26, 27};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(m_encoder.GetEncoderRotation());
}