#include "Thermistor.h"

#include <Arduino.h>

using namespace lattice;

// cf. https://github.com/panStamp/thermistor
// cf. https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/NTCLE350E4103FHB0/12608841

Thermistor::Thermistor(const int PIN) : M_PIN(PIN) {}

double Thermistor::Get() {
    double resistance = GetResistance();
    // cf. https://en.wikipedia.org/wiki/Thermistor#B_or_%CE%B2_parameter_equation
    // The constants come from the nominal temperature of 25 deg C, and the conversion
    // between Kelvin and Celsius (273.15)
    return 1.0 / (1.0 / 298.15 + log(resistance / NOMINAL_RESISTANCE) / BETA_COEFF) - 273.15;
}

double Thermistor::GetResistance() {
    double avg = 0;
    for (int i = 0; i < NUM_READINGS; i++) {
        avg += analogRead(M_PIN);
        delay(DELAY);
    }
    avg = avg / NUM_READINGS;

    // convert to resistance
    return SERIES_RESISTANCE * (MAX_VAL / avg - 1);
}