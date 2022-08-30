#include <Arduino.h>

#include "IMU.h"
#include "Util.h"

lattice::IMU imu1;
lattice::IMU imu2{lattice::WireType::kOne};

void setup() {
    lattice::GenericSetup();
    imu1.Setup();
    imu2.Setup();
}

void printIMU(lattice::IMU imu) {
    Serial.print(imu.GetYaw());
    Serial.print(", ");
    Serial.print(imu.GetPitch());
    Serial.print(", ");
    Serial.print(imu.GetRoll());
    Serial.print(", ");
    Serial.print(imu.GetXAccel());
    Serial.print(", ");
    Serial.print(imu.GetYAccel());
    Serial.print(", ");
    Serial.print(imu.GetZAccel());
    Serial.println();
}

void loop() {
    // printIMU(imu1);
    printIMU(imu2);
}