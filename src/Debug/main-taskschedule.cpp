#include <Arduino.h>
#include <TaskScheduler.h>

Scheduler ts;

void testFunc() {
    Serial.println(millis(), 1);
}

constexpr int period = 5;

Task mainLoop(period, TASK_FOREVER, &testFunc);

void setup() {
    Serial.begin(115200);
    Serial.println("Scheduler TEST");

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