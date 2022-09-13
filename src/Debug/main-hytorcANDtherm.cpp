#include <Arduino.h>

#include "HytorcSimple.h"
#include "Thermistor.h"
#include "Util.h"

lattice::HytorcSimple testMotor{9, 5, 6};
lattice::Thermistor thermistor(3);

double power = 0.0;
float power_scaling = 1;

double resting_temp = 40;
double hytorc_temp_bound = resting_temp + 45;
// these values are in Celsius

unsigned long time = 0;
unsigned long time_old = 0;
bool SHUT_OFF = false;
int shut_off_loop_count = 0;

void setup() {
    lattice::GenericSetup();
    testMotor.Setup();
    testMotor.SetPercentOutput(0.0);
}

void loop(){

    time = millis();
    power = 0.25;
    // declare our desired power output

    float temp = thermistor.Get();
    if ((temp < hytorc_temp_bound) and (SHUT_OFF == false)){
        power_scaling = 1 - (1 / (1 + pow(EULER,-2 * (temp - hytorc_temp_bound))));
        power = power * power_scaling;
        // calculate the temperature_adjusted power output
    }
    if ((temp >= hytorc_temp_bound) or (SHUT_OFF == true)){
        SHUT_OFF = true;
        if (time - time_old > 50){
            if (shut_off_loop_count < 10){
                shut_off_loop_count += 1;
            }
            power = power * (1 - (shut_off_loop_count / 10));
            time_old = time;
        }
        // shutdown hytorc if temperature exceeds limit
    }
    
    // execute hytorc output here
}