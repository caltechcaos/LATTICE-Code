#include <Arduino.h>
#include "Util.h"
#include "Brake.h"
#include "HytorcSimple.h"

lattice::Brake brakeport(lattice::ShuttleConstants::kBrakePin);
lattice::HytorcSimple testMotor(9, 5, 6);

double power = 0;
bool brakeflag = false; 
//temperary until test
//Brake = false (off)
//brake = true (on)
 
void setup(){
    testMotor.Setup();
    testMotor.SetPercentOutput(power);
    brakeport.Setup();
}

void test(){    
    if(Serial.available()){
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            power = 0.25;
        } else if (input == 'b') {
            power = -0.25;
        } else if (input == 'c') {
            power = 1;
        } else if (input == 'd') {
            power = -1;
        } else if (input == 'f') {
            power = 0;
        }
        if(input == 'j'){
            brakeflag = true;
            power = 0;
        }
    }

    Serial.println(testMotor.GetPosition());
    testMotor.SetPercentOutput(power);
    brakeport.Set(brakeflag);
}