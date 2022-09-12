#include <Arduino.h>

#include "ElevatorMotor.h"
#include "Util.h"
#include "RC.h"

lattice::RC controller(Serial1, 19, 2, 13);
lattice::ElevatorMotor testMotor{12, 2, 3};
void setup() {
    lattice::GenericSetup();
    controller.Setup();
    testMotor.Setup();
    testMotor.Run(0.0);
}

double power = 0.0;

void loop() {
    controller.Update();
    int gear = controller.GetGear();
    double elev = controller.GetElevator();
    if (gear ==1)
    {
        if (elev > 0.75) {
            power = 0.25;
        } else if (elev < 0.25) {
            power = -0.25;
        } else {
            power = 0;
        }
    }
    else{
        power = 0;
    }
    //if (Serial.available()) {
        //else if (input == 'c') {
          //  power = 1;
        //} else if (input == 'd') {
        //    power = -1;
    //}
    Serial.println(gear);
    //Serial.println(testMotor.GetPosition());

    testMotor.Run(power);
}