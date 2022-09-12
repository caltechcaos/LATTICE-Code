#include "Voltage.h"
#include <Arduino.h>

using namespace lattice;

Voltage::Voltage(const int PIN) : M_PIN(PIN) { }
 double temp = 0;
void Voltage::Setup() {
    pinMode(M_PIN, INPUT);
}
double Voltage::CalculateVoltage(double MaxInputVoltage, double Multiplier) {
    //digitalWrite(M_PIN, value ? HIGH : LOW);
    ReadValue = analogRead(M_PIN);
    CurrentVoltage = (ReadValue * MaxInputVoltage) ;
    temp = (CurrentVoltage * Multiplier) / Divisor;
    return temp;
}
