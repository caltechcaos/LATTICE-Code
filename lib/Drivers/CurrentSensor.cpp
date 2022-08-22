#include "CurrentSensor.h"

#include <Arduino.h>

#include "Constants.h"

using namespace lattice;

CurrentSensor::CurrentSensor(const int PIN) : M_PIN(PIN) {}

double CurrentSensor::Get() {
    double val = (double)analogRead(M_PIN);  // arbitrary units
    val = val / MAX_ANALOG * MAX_VOLTAGE;    // voltage
    return val / VOLT_TO_CURR;               // current
}