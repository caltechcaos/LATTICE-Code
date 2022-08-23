#include "IMU.h"

using namespace lattice;

IMU::IMU(WireType wireType) : kI2CWireType(wireType) {}

bool IMU::Setup() {
    delay(500);
    bool success;
    if (kI2CWireType == kOne) {
        Wire1.begin();
        Wire1.setClock(400000UL);  // I2C frequency, 400kHz
        success = mIMU.begin(DEFAULT_ADDRESS, Wire1);
    } else {
        Wire.begin();
        Wire.setClock(400000UL);  // I2C frequency, 400kHz
        success = mIMU.begin(DEFAULT_ADDRESS, Wire);
    }
    success &= mIMU.initialize(BASIC_SETTINGS);
    return success;
}
double IMU::GetRoll() {
    return mIMU.readFloatGyroX();
}
double IMU::GetPitch() {
    return mIMU.readFloatGyroY();
}
double IMU::GetYaw() {
    return mIMU.readFloatGyroZ();
}
double IMU::GetXAccel() {
    return mIMU.readFloatAccelX();
}
double IMU::GetYAccel() {
    return mIMU.readFloatAccelY();
}
double IMU::GetZAccel() {
    return mIMU.readFloatAccelZ();
}