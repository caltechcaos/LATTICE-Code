#include <Arduino.h>

#include "Util.h"
#include "Subsystems/Driver.h"

using namespace lattice;
using namespace lattice::HandoffConstants;

auto& driver = lattice::Driver::driver();
int steps; 
bool enable = false;

void setup() {
    lattice::GenericSetup();
    driver.Setup();
}

void loop() {
    driver.UpdateSensors();
    if(Serial.available()){
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            driver.SetStake(lattice::Driver::StakeNumber::kOne);
            driver.InitializeStakeHandoff();
        } else if (input == 'b') {
            driver.SetStake(lattice::Driver::StakeNumber::kTwo);
            driver.InitializeStakeHandoff();
        } else if (input == 'c') {
            driver.SetStake(lattice::Driver::StakeNumber::kThree);
            driver.InitializeStakeHandoff();
        } else if (input == 'e') {
            enable = true;
        } else if (input == 'f') {
            enable = false;
        }

    }
    
    if (enable) {
        driver.RunStakeHandoff();
    } else {
        driver.EStop();
    }

}
