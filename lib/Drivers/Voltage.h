#pragma once

#include "CircularBuffer.h"

namespace lattice {
/*
 * This code is to measure the battery voltage
 * and report it somewhere
 * Using resistors 12k OHM and 75k OHM
 */
class Voltage {
   public:
    /**
     * Creates a pin input on the given pin
     * @param PIN
     */
    Voltage(const int PIN, double resistanceL = 75000.0, double resistanceS = 12000.0);

    /**
     * Sets up to use as input for votage. Must be called before
     * using it.
     */
    void Setup();
    /**
     * Calculates the volatge based on resistors
     * @param value will multiply based on what resistor value
     */
    double CalculateVoltage();

    double CalculateFilteredVoltage();

   private:
    double GetMean();
    const int M_PIN;
    double ReadValue;
    double CurrentVoltage;
    const double kLargeResistor;
    const double kSmallResistor;
    const double kResistanceCoefficient;
    CircularBuffer<double, 10> mBuffer;
    // double MilliVoltsPerCount = 4.55; //temperary
};
}  // namespace lattice