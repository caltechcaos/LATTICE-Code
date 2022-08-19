#include "MotionMotor.h"

#include <Arduino.h>

#include <cmath>

using namespace lattice;

MotionMotor::MotionMotor(int enablePort, int pwmPort, int analogInPort, double maxRPM, double resolution) : kEnablePort(enablePort), kPWMPort(pwmPort), kAnalogInputPort(analogInPort), kMaxRPM(maxRPM), kBitResolution(pow(2, resolution) - 1) {
    pinMode(kEnablePort, OUTPUT);
    digitalWrite(kEnablePort, LOW);
}
void MotionMotor::EnableMotor() {
    digitalWrite(kEnablePort, HIGH);
}
void MotionMotor::DisableMotor() {
    digitalWrite(kEnablePort, LOW);
}
void MotionMotor::SetVelocity(double rpm) {
    double PWMOutput = ScalePercentOutput(rpm) * kBitResolution;
    analogWrite(kPWMPort, PWMOutput);
}
double MotionMotor::GetVelocity() {
    return ScaleInput((double)analogRead(kAnalogInputPort) / kBitResolution);
}

double MotionMotor::ScalePercentOutput(double rpm) {
    double percentOutput = rpm / kMaxRPM / 2;
    return (0.5 + percentOutput) * 0.8 + .1;
}
double MotionMotor::ScaleInput(double percentInput) {
    return kMaxRPM * ((percentInput)*2 - 1);
}