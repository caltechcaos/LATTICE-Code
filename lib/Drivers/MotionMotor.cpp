#include "MotionMotor.h"

#include <Arduino.h>

#include <cmath>

using namespace lattice;

MotionMotor::MotionMotor(int enablePort, int pwmPort, int analogInPort, double maxRPM, double resolution, double deadband)
    : kEnablePort(enablePort), kPWMPort(pwmPort), kAnalogInputPort(analogInPort), kMaxRPM(maxRPM), kBitResolution(pow(2, resolution) - 1), kDeadband(deadband) {
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
    double percentOutput = rpm / kMaxRPM / 2.0;                      // Splits percent output by half since we are considering negatives
    return (0.5 + percentOutput) * (1 - kDeadband * 2) + kDeadband;  // Puts in deadband range of the motor
}
double MotionMotor::ScaleInput(double percentInput) {
    // max_rpm * percent_input * 2 - max_rpm = max_rpm * (percent_input * 2 - 1).
    // This allows the [0, 2 * max_rpm] input to be fitted to positive and negative counterparts as positive values stay positive (0, max_rpm] while negative values go negative and zero goes to zero.
    return kMaxRPM * ((percentInput)*2 - 1);
}