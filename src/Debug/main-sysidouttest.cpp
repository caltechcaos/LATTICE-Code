#include <Arduino.h>
#include <TaskScheduler.h>

#include "Util.h"

constexpr double rampRate = 0.25;       // V/s
constexpr double dynamicVoltage = 6.0;  // V

int dir = 1;
long startTime = 0;
double targetVoltage = 0.0;
double measuredPos = 0.0;
bool quasistatic = true;
bool enabled = false;

void printTestStart(bool quasistatic, int dir) {
    Serial.print("STARTING-");
    if (quasistatic) {
        Serial.print("QUASISTATIC ");
    } else {
        Serial.print("DYNAMIC ");
    }
    if (dir == 1) {
        Serial.println("FORWARD");
    } else {
        Serial.println("BACKWARD");
    }
}

void printTelemetry(double currTime, double voltage, double position) {
    Serial.print(currTime);
    Serial.print(", ");
    Serial.print(voltage, 4);
    Serial.print(", ");
    Serial.print(position, 4);
    Serial.println();
}

double getVoltageCommand(bool quasistatic, double rampRate, double dynamicVoltage, long startTime, long currTime) {
    if (quasistatic) {
        return (currTime - startTime) / 1000.0 * rampRate;
    } else {
        return dynamicVoltage;
    }
}

void run() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();

        if (input == 's') {  // Start test
            enabled = true;
            startTime = millis();
            printTestStart(quasistatic, dir);
        }
        if (input == 'e') {  // End test
            enabled = false;
            Serial.println("DISABLING");
        }
        if (input == 'q') {  // Quasistatic Mode
            quasistatic = true;
        }
        if (input == 'd') {  // Dynamic Mode
            quasistatic = false;
        }
        if (input == 'f') {  // Forward
            dir = 1;
        }
        if (input == 'b') {  // Backward
            dir = -1;
        }
    }
    long currTime = millis();
    if (enabled) {
        targetVoltage = dir * getVoltageCommand(quasistatic, rampRate, dynamicVoltage, startTime, currTime);
    } else {
        targetVoltage = 0.0;
    }
    // Do something with voltage and read position
    measuredPos = targetVoltage;

    printTelemetry(currTime, targetVoltage, measuredPos);
}

constexpr int period = 5;
Scheduler ts;

Task mainLoop(period, TASK_FOREVER, &run);

void setup() {
    lattice::GenericSetup();
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}