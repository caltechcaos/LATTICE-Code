#include "HytorcSimple.h"

#include <Arduino.h>
#include <Servo.h>

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
    SetPercentOutput(desiredVoltage / batteryVoltage);
}

double HytorcSimple::GetPosition() {
    return (kInvert ? 1 : -1) * mMotorEncoder.read() / kCPR;  // Currently, the default hall behaviour is clockwise negative so we negate the sign in the non-invert case
}

void HytorcSimple::ResetEncoderPosition() {
    mMotorEncoder.write(0);
}