#pragma once

namespace lattice {
class CurrentSensor {
   public:
    /**
     * Creates a current sensor on the given analog pin
     */
    CurrentSensor(const int PIN);

    /**
     * Returns the current measured by the sensor
     * @return Current, in amps
     */
    double Get();

   private:
    const int M_PIN;
    /**
     * cf. https://cdn.sparkfun.com/assets/c/3/1/f/e/Current_to_Voltage_90A.png
     * cf. https://www.sparkfun.com/products/16408
     */
    const double VOLT_TO_CURR = 0.0366;
};
}  // namespace lattice