#include "HytorcRC.h"

#include <Arduino.h>
#include <Servo.h>

#include "Constants.h"
using namespace lattice;

HytorcRC::HytorcRC(int pwmPort, int forwardPort, int backwardPort) : kPWMPort(pwmPort), mMotorEncoder(forwardPort, backwardPort) {}
void HytorcRC::Setup() {
    mMotor.attach(kPWMPort);
}
void HytorcRC::SetPercentOutput(double percentOut) {
    mMotor.writeMicroseconds(1000 + 1000 * (percentOut * 0.5 + 0.5));
}
void HytorcRC::SetVoltage(double desiredVoltage, double batteryVoltage) {
    SetPercentOutput(desiredVoltage / batteryVoltage);
}

double HytorcRC::GetEncoderRotation() {
    return -mMotorEncoder.read() / kCPR;
}