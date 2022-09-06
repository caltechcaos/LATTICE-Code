#include "Clifford.h"

#include "Util.h"

using namespace lattice;

Clifford::Clifford()
    : brake(false), joystick(), trigger(CliffordConstants::kJoystickTriggerPin) {}

void Clifford::Setup() {
    joystick.Setup();
    trigger.Setup();

    Zero();
    trigger.Set(true);
}

bool Clifford::SetBrake(bool brake) {
    if (brake == Clifford::brake) {
        return false;
    }

    if (brake) {
        Zero();  // stop movement
        trigger.Set(false);
    } else {
        trigger.Set(true);
    }
    Clifford::brake = brake;
    return true;
}

void Clifford::Zero() {
    Move(0.5, 0.5);
}

bool Clifford::Move(double x, double y) {
    if (brake || x < 0 || y < 0 || x > 1 || y > 1) {
        return false;
    }

    if (!joystick.Set1(x * Digipot::DIGIPOT_MAX) ||
        !joystick.Set2(y * Digipot::DIGIPOT_MAX)) {
        return false;
    }

    return true;
}