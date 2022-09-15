#include <Arduino.h>

#include "Subsystems/Driver.h"
#include "TaskScheduler.h"
#include "Util.h"
using namespace lattice;
using namespace lattice::HandoffConstants;

auto& driver = lattice::Driver::driver();
int steps;
bool enable = false;

Scheduler ts;

void run() {
    driver.UpdateSensors();
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            driver.SetStake(lattice::Driver::StakeNumber::kOne);
            driver.InitializeStakeHandoff();
            Serial.println("1");
        } else if (input == 'b') {
            driver.SetStake(lattice::Driver::StakeNumber::kTwo);
            driver.InitializeStakeHandoff();
            Serial.println("2");
        } else if (input == 'c') {
            driver.SetStake(lattice::Driver::StakeNumber::kThree);
            driver.InitializeStakeHandoff();
            Serial.println("3");
        } else if (input == 'e') {
            enable = true;
        } else if (input == 'f') {
            enable = false;
        }
    }

    if (enable) {
        Serial.println(driver.RunStakeHandoff());
    } else {
        driver.EStop();
        Serial.println("00000");
    }
    // if (enable) {
    //     Serial.println("000");
    // } else {
    //     Serial.println("---");
    // }
}

constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &run);

void setup() {
    lattice::GenericSetup();
    driver.Setup();
    ts.init();
    Serial.println("Initialized scheduler");

    ts.addTask(mainLoop);
    Serial.println("added main task");
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}
