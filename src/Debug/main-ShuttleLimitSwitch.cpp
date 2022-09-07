#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "TaskScheduler.h"
#include "Util.h"

// Manual Mode -> Test if limit switches work to cut power and if normal control works
// Left Arm Transition
// Both Arm Transition
// Right Arm Transition

auto& shuttle = lattice::Shuttle::GetInstance();
lattice::RC controller{Serial1, 19, 2, 13};
int testState = 0;
constexpr double takeup = 0.25;
constexpr int kPeriod = 20;
double power = 0;
void run() {
    shuttle.UpdateSensors();
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();
        if (input == 'a') {
            power = 5;
            Serial.println("1");
        } else if (input == 'b') {
            power = -5;
            Serial.println("2");
        } else if (input == 'e') {
            power = 0;
            Serial.println("3");
        }
    }

    shuttle.SetTensionArmPowers(-power, power);
    Serial.print(power);
    Serial.print(", ");
    Serial.print(shuttle.GetLeftTensionArmPos());
    Serial.print(", ");
    Serial.println(shuttle.GetRightTensionArmPos());
}

Task mainLoop(kPeriod, TASK_FOREVER, &run);
Scheduler ts;
void setup() {
    lattice::GenericSetup();
    shuttle.Setup();
    shuttle.SetTakeup(takeup);
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
}

void loop() {
    ts.execute();
}