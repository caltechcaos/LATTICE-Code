#include "HytorcSimple.h"

#include <Arduino.h>
#include <Servo.h>

//#include <algorithm>
#include <cmath>

#include "Constants.h"
using namespace lattice;

HytorcSimple::HytorcSimple(int pwmPort, int forwardPort, int backwardPort, bool invert, double gearing) : kPWMPort(pwmPort), mMotorEncoder(forwardPort, backwardPort), kInvert(invert), kGearing(gearing) {}
void HytorcSimple::Setup() {
    mMotor.attach(kPWMPort);
}
void HytorcSimple::SetPercentOutput(double percentOut) {
    percentOut *= kInvert ? -1 : 1;
    mMotor.writeMicroseconds(1000 + 1000 * (percentOut * 0.5 + 0.5));
}
void HytorcSimple::SetVoltage(double desiredVoltage, double batteryVoltage) {
    double actualVoltage = abs(desiredVoltage) > batteryVoltage ? batteryVoltage : desiredVoltage;
    desiredVoltage = std::copysign(actualVoltage, desiredVoltage);
    SetPercentOutput(desiredVoltage / batteryVoltage);
}

double HytorcSimple::GetPosition() {
    return (kInvert ? -1 : 1) * mMotorEncoder.read() / kCPR;
}

void HytorcSimple::ResetEncoderPosition() {
    mMotorEncoder.write(0);
}