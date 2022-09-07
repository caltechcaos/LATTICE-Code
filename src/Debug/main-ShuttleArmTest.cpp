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
int state = 0;
constexpr double takeup = 0.5;
constexpr int kPeriod = 20;

int state = 0;
void run() {
    if (Serial.available()) {
        char input = (uint8_t)Serial.read();
        if (input == 'a') {
            state = 1;
        } else if (input == 'b') {
            state = 2;
        } else if (input == 'c') {
            state = 3;
        } else if (input == 'e') {
            state = 0;
        }
    }

    if (state == 1) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kBothArmsRaised)) {
            state = 0;
        }
    } else if (state == 2) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kLeftArmRaised)) {
            state = 0;
        }
    } else if (state == 3) {
        if (shuttle.ArmTransition(lattice::Shuttle::ArmTransitionPositions::kRightArmRaised)) {
            state = 0;
        }
    } else {
        shuttle.SetTensionArmPowers(0, 0);
    }
    Serial.print(shuttle.GetLeftTensionArmPos());
    Serial.print(", ");
    Serial.println(shuttle.GetRightTensionArmPos());
}

Task mainLoop(kPeriod, TASK_FOREVER, &run);
Scheduler ts;
void setup() {
    lattice::GenericSetup();
    shuttle.Setup();
    ts.addTask(mainLoop);
    delay(500);
    mainLoop.enable();
}

void run() {
    ts.execute();
}