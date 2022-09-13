#include <Arduino.h>
#include "Voltage.h"
#include "Util.h"

lattice::Voltage ShuttleVoltage(lattice::ShuttleConstants::kVoltagePin);
int average_count = 0;
unsigned long old_time = 0;
unsigned long new_time = 0;
float voltage = 0.0;
float average = 0;

// float counts = 0;
// float mv = 0;

// const float mvc = 3.69; input voltage divided by 1024


void setup() {
    lattice::GenericSetup();
    ShuttleVoltage.Setup();
}

// the loop function runs over and over again forever
void loop() {
    //Serial.println(analogRead(lattice::ShuttleConstants::kVoltagePin));
    new_time = millis();
    
    if (new_time - old_time > 100){
        voltage = ShuttleVoltage.CalculateVoltage(lattice::ShuttleConstants::kMaxInputVoltage,
     lattice::ShuttleConstants::kMultiplier);
        average_count += 1;
        average += voltage;
        old_time = new_time;
    }

    if (average_count == 10){
        Serial.println(average / 10);
        average_count = 0;
        average = 0;
    }
    
    // counts = analogRead(lattice::ShuttleConstants::kVoltagePin);
    // Serial.println(String(analogRead(counts)));

}
