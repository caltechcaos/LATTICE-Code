#include "RC.h"
#include "Subsystems/Shuttle.h"
#include "TaskScheduler.h"
#include "Util.h"

// Manual Mode -> Test if limit switches work to cut power and if normal control works
// Left Arm Transition
// Both Arm Transition
// Right Arm Transition

auto& shuttle = lattice::Shuttle::GetInstance();
// lattice::RC controller{Serial1, 19, 2, 13};
int testState = 0;
constexpr double takeup = 0.25;
constexpr int kPeriod = 20;

void run() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();
        if (input == 'a') {
            testState = 1;
            shuttle.StartArmTransition(lattice::Shuttle::kBothArmsRaised);
        } else if (input == 'b') {
            testState = 2;
            shuttle.StartArmTransition(lattice::Shuttle::kLeftArmRaised);
        } else if (input == 'c') {
            testState = 3;
            shuttle.StartArmTransition(lattice::Shuttle::kRightArmRaised);
        } else if (input == 'd') {
            testState = 4;
            shuttle.StartArmTransition(lattice::Shuttle::kBothArmsRaisedNormal);
        } else if (input == 'e') {
            testState = 0;
        } else if (input == 'r') {
            testState = 0;
            shuttle.ResetArmPositions();
        }
    }

    if (testState == 1) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kBothArmsRaised)) {
            testState = 0;
        }
    } else if (testState == 2) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kLeftArmRaised)) {
            testState = 0;
        }
    } else if (testState == 3) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kRightArmRaised)) {
            testState = 0;
        }
    } else if (testState == 4) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kBothArmsRaisedNormal)) {
            testState = 0;
        }
    } else {
        shuttle.SetTensionArmPowers(0, 0);
    }
    Serial.print(shuttle.GetLeftSetpoint());
    Serial.print(", ");
    Serial.print(shuttle.GetRightSetpoint());
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