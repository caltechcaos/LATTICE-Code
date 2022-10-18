#include <Arduino.h>

#include "ElevatorMotor.h"
#include "RC.h"
#include "Util.h"
lattice::ElevatorMotor testMotor{48, 2, 3};
#include <Arduino.h>
#include <TaskScheduler.h>

Scheduler ts;

double power = 0.0;

void testFunc() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 'a') {
            power = 0.8;
        } else if (input == 'b') {
            power = 1;
        } else if (input == 'c') {
            power = 2;
        } else if (input == 'd') {
            power = -.8;
        } else if (input == 'f') {
            power = 0;
        }
    }
    Serial.println(power);

    testMotor.SetVoltage(power, 22);
}

constexpr int period = 20;

Task mainLoop(period, TASK_FOREVER, &testFunc);

void setup() {
    lattice::GenericSetup();
    testMotor.Setup();
    testMotor.Run(0.0);

    ts.init();
    Serial.println("Initialized scheduler");

    ts.addTask(mainLoop);
    Serial.println("added main task");
    delay(5000);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}