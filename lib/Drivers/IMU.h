#pragma once

#include "SparkFunLSM6DSO.h"
#include "WireType.h"

namespace lattice {
class IMU {
   public:
    /**
     * Constructor for the Spark Fun LSM6DSO IMU
     *
     * @param wireType The type of I2C Wire to use. See WireType for more details.
     */
    IMU(WireType wireType = kDefault);

    /**
     * Sets up the IMU
     *
     * @return True if the setup was successful
     */
    bool Setup();

    /**
     * Gets the Roll of the IMU or rotation around the x axis.
     *
     * @return The roll of the IMU.
     */
    double GetRoll();

    /**
     * Gets the Pitch of the IMU or rotation around the y axis.
     *
     * @return The pitch of the IMU.
     */
    double GetPitch();

    /**
     * Gets the Yaw of the IMU or rotation around the z axis.
     *
     * @return The yaw of the IMU.
     */
    double GetYaw();

    /**
     * Gets the acceleration of the IMU in the x axis.
     *
     * @return The x-axis acceleration of the IMU.
     */
    double GetXAccel();

    /**
     * Gets the acceleration of the IMU in the y axis.
     *
     * @return The y-axis acceleration of the IMU.
     */
    double GetYAccel();

    /**
     * Gets the acceleration of the IMU in the z axis.
     *
     * @return The z-axis acceleration of the IMU.
     */
    double GetZAccel();

   private:
    LSM6DSO mIMU;
    WireType kI2CWireType;
};
}  // namespace lattice