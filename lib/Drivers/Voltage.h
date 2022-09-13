#pragma once

namespace lattice{
/* 
 * This code is to measure the battery voltage 
 * and report it somewhere
 * Using resistors 12k OHM and 75k OHM 
 */
class Voltage{
    public:
    /**
     * Creates a pin input on the given pin
     * @param PIN 
     */
    Voltage(const int PIN);

    /**
     * Sets up to use as input for votage. Must be called before
     * using it.
     */
    void Setup();
    /**
     * Calculates the volatge based on resistors
     * @param value will multiply based on what resistor value
     */
    double CalculateVoltage(double MaxInputVoltage, double Multiplier);
        
    private:
     const int M_PIN;
     double ReadValue;
     double CurrentVoltage;
     const double Large_Resistor = 75000;
     const double Small_Resistor = 12000;
     const double Resistance_Coefficient = (Large_Resistor + Small_Resistor) / Small_Resistor;
     //double MilliVoltsPerCount = 4.55; //temperary
    };
}