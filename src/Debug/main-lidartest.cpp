#include <Arduino.h>

#include "Lidar.h"

lattice::Lidar lidar1{};
lattice::Lidar lidar2{lattice::WireType::kOne};
void setup() {
    Serial.begin(115200);
    lidar1.Setup();
    lidar2.Setup();
}

void loop() {
    lidar1.Update();
    lidar2.Update();
    // Serial.print("Lidar (SCL/SDA): ");
    // Serial.print(lidar1.GetDistance());
    // Serial.print(", ");
    // Serial.println(lidar1.GetTimestamp());

    Serial.print("Lidar (SCL1/SDA1): ");
    Serial.print(lidar2.GetDistance());
    Serial.print(", ");
    Serial.println(lidar2.GetTimestamp());
}