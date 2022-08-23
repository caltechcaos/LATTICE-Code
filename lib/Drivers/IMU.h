#pragma once

#include "SparkFunLSM6DSO.h"

namespace lattice {
class IMU {
   public:
    enum WireType {
        kDefault,
        kOne
    };
    IMU();
    void Setup();
    double GetRoll();
    double GetPitch();
    double GetYaw();
    double GetXAccel();
    double GetYAccel();
    double GetZAccel();

   private:
};
}  // namespace lattice