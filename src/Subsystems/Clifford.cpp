#include "Clifford.h"
#include <Arduino.h>

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

    if (!XAxisMove(x)) {
        if (!YAxisMove(y)){
            
        }
        return false;
    }

    // if (!joystick.Set1(x * Digipot::DIGIPOT_MAX) ||
    //     !joystick.Set2(y * Digipot::DIGIPOT_MAX)) {
    //     return false;
    // }

    return true;
}
bool Clifford::XAxisMove(double x){
    // deadband of .05
    if(x >= 0.52 || x <= 0.48) {
        joystick.Set1(x * Digipot::DIGIPOT_MAX);
        Serial.print("moving at x: ");
        Serial.println(x);
        return false;
    } else {
        joystick.Set1(128);
        return false;
    } 
    return true;
}
bool Clifford::YAxisMove(double y){
    // deadband of .05
    if(y >= 0.52 || y <= 0.48){
        joystick.Set2(y * Digipot::DIGIPOT_MAX);
        Serial.print("moving at y: ");
        Serial.println(y);
        return false;
    } else{
        joystick.Set2(128);
        return false;
    }
    return true;
}
