#include "PIDF.h"

#ifdef ARDUINO_SAM_DUE
#include <Arduino.h>
#endif
using namespace lattice;

PIDF::PIDF(const double kP, const double kI, const double kD,
           const std::function<double(double)> mLambdaf, double errorThreshold, double derivThreshold, double period)
    : kP(kP), kI(kI), kD(kD), mLambdaf(mLambdaf), kMaxError(errorThreshold), kMaxDerivError(derivThreshold), kPeriod(period) {}

PIDF::PIDF(const double kP, const double kI, const double kD, const double kF, double errorThreshold, double derivThreshold, double period)
    : kP(kP), kI(kI), kD(kD), mLambdaf([=](double setpoint) { return kF; }), kMaxError(errorThreshold), kMaxDerivError(derivThreshold), kPeriod(period) {}

double PIDF::Run(double feedback) {
    mHistory.Add(feedback);
    double error = mSetpoint - feedback;
    // TODO Add logger error handling and reporting
    // We are subtracing the Derivative term as we are assuming the target derivative setpoint is 0 thus (0 - Derivative) is what the gain should apply to
    double input = kP * error + kI * CalcI() - kD * CalcD() + mLambdaf(mSetpoint);
    return input;
}

double PIDF::Run(double feedback, double setpoint) {
    mSetpoint = setpoint;
    return Run(feedback);
}

void PIDF::SetTarget(double setpoint) {
    mSetpoint = setpoint;
}

double PIDF::CalcI() {
    // First order numerical differentiation (Euler integration)
    mAccum += (mSetpoint - mHistory.Get(0)) * kPeriod;
    return mAccum;
}

double PIDF::CalcD() {
    // Implementing second order backward finite difference stated here: https://www.dam.brown.edu/people/alcyew/handouts/numdiff.pdf
    // (3f(x) − 4f(x − ∆x) + f(x − 2∆x) / (2∆x)
    // Note this will be just the derivaitve instead of derivative of error to reduce derivative kick from moving setpoints
    auto output = (3 * mHistory.Get(0) - 4 * mHistory.Get(1) + mHistory.Get(2)) / (2 * kPeriod);
    return output;
}

bool PIDF::AtTarget() {
    // Serial.println(mHistory.Get(0));
    double error = abs(mSetpoint - mHistory.Get(0));
    return error < kMaxError && abs(CalcD()) < kMaxDerivError;
}