#pragma once

namespace lattice {
/**
 * Class to interface with a hall effect sensor.
 *
 * Note that the sensor turns on when it detects a magnetic field and then turns off when the other pole of the field is detected.
 */
class HallEffect {
   public:
    /**
     * Constructor for the hall effect sensor
     *
     * @param signalPort The port on the Due that receives the signal
     */
    HallEffect(int signalPort);

    /**
     * Sets up the hall effect sensor
     */
    void Setup();

    /**
     * Gets if the sensor is triggered
     *
     * @return True if the sensor is triggered/on, false if the sensor is not triggered/off.
     */
    bool Get();

   private:
    const int kPort;
};
}  // namespace lattice