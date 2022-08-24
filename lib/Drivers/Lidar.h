#pragma once
#include <Arduino.h>

#include "LIDARLite_v4LED.h"
#include "WireType.h"

namespace lattice {
class Lidar {
   public:
    /**
     * Constructor for the Garmin LIDARLite v4
     *
     * @param type The type of I2C Port to plug the lidarlite into
     */
    Lidar(WireType type = kDefault);

    /**
     * Setup function for the LIDAR
     *
     * @return If the setup was successful
     */
    bool Setup();

    /**
     * Updates the LIDAR reading.
     * Call this EVERY loop cycle.
     */
    void Update();

    /**
     * Gets the distance of the LIDAR in the nearest centimeter
     *
     * @returns The distance measurement of the lidar in the nearest centimeter.
     */
    double GetDistance();

    /**
     * Gets the timestamp of the latest LIDAR measurement
     *
     * @return The timestamp of the latest LIDAR measurement
     */
    long GetTimestamp();

   private:
    LIDARLite_v4LED mLidar;
    WireType kI2CType;
    int mDist = 0;
    long mTimestamp = 0;
};
}  // namespace lattice