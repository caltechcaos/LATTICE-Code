#pragma once

namespace lattice {
    class Thermistor {
        public:
        /**
         * Constructs a thermistor for the given pin
         */
        Thermistor(const int PIN);

        /**
         * Gets the temperature of the thermistor using the Steinhart–Hart equation
         * @returns The temperature in degrees C, to 0.1 deg precision
         */
        double Get();

        private:
        const int M_PIN;
        // When averaging results from the thermistor
        static constexpr double NUM_READINGS = 10.0; 
        static const int DELAY = 10; // In milliseconds   
        static constexpr double MAX_VAL = 1023.0;     

        /**
         * Known properties of our circuitry
         */
        static constexpr double NOMINAL_RESISTANCE = 10000.0; // At 25 deg C, in ohms
        static constexpr double BETA_COEFF = 3984.0; // B25/85, in Kelvin
        static constexpr double SERIES_RESISTANCE = 20000.0; // In ohms

        /**
         * Reads NUM_READINGS values, with a DELAY delay between each,
         * converts the result to resistance in ohms, and returns it
         */
        double GetResistance();
    };
}