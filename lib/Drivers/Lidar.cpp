#include "Lidar.h"
using namespace lattice;

Lidar::Lidar(WireType type) : kI2CType(type) {}
bool Lidar::Setup() {
    delay(500);
    bool success;
    if (kI2CType == kOne) {
        Wire1.begin();
        Wire1.setClock(400000UL);  // I2C frequency, 400kHz
        digitalWrite(SCL1, LOW);
        digitalWrite(SDA1, LOW);
        success = mLidar.begin(LIDARLITE_ADDR_DEFAULT, Wire1);
    } else {
        Wire.begin();
        Wire.setClock(400000UL);  // I2C frequency, 400kHz
        digitalWrite(SCL, LOW);
        digitalWrite(SDA, LOW);
        success = mLidar.begin(LIDARLITE_ADDR_DEFAULT, Wire);
    }
    mLidar.configure(0);
    return success;
}
void Lidar::Update() {
    if (!mLidar.getBusyFlag()) {
        mLidar.takeRange();
        mDist = mLidar.readDistance();
        mTimestamp = millis();
    }
}
double Lidar::GetDistance() {
    return mDist;
}
long Lidar::GetTimestamp() {
    return mTimestamp;
}