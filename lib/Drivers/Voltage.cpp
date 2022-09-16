#include "Voltage.h"

#include <Arduino.h>

using namespace lattice;

static constexpr double kArduinoAnalogVoltage = 3.3;
static constexpr int kArduinoBitSize = 4096;

Voltage::Voltage(const int PIN, double resistanceL, double resistanceS) : M_PIN(PIN), kLargeResistor(resistanceL), kSmallResistor(resistanceS), kResistanceCoefficient((kLargeResistor + kSmallResistor) / kSmallResistor) {}

void Voltage::Setup() {
    pinMode(M_PIN, INPUT);
}
double Voltage::CalculateVoltage() {
    ReadValue = analogRead(M_PIN);
    return kArduinoAnalogVoltage * ReadValue / kArduinoBitSize * kResistanceCoefficient;
}

double Voltage::CalculateFilteredVoltage() {
    double voltage = CalculateVoltage();
    mBuffer.Add(voltage);
    return GetMean();
}

double Voltage::GetMean() {
    double sum = 0.0;
    for (int i = 0; i < mBuffer.Size(); i++) {
        sum += mBuffer[i];
    }
    return sum / mBuffer.Size();
}
