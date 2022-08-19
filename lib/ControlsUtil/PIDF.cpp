#include "PIDF.h"

using namespace lattice;

PIDF::PIDF(const double kP, const double kI, const double kD,
        const std::function<double(double)> mLambdaf)
        : kP(kP), kI(kI), kD(kD), mLambdaf(mLambdaf) { }

PIDF::PIDF(const double kP, const double kI, const double kD, const double kF)
        : kP(kP), kI(kI), kD(kD), mLambdaf([=](double setpoint) {return kF;} ) { }

// TODO: add time elapsed
std::tuple<double, bool> PIDF::Run(double feedback, double setpoint) {
    double error = setpoint - feedback;
    mHistory.Add(error);

    double input = kP * error + kI * CalcI() + kD * CalcD() + mLambdaf(setpoint);

    return {input, true};
}

double PIDF::CalcI() {
    mAccum += mHistory.Get(0); // multiplied by time elapsed
    return mAccum;
}

double PIDF::CalcD() {
    return mHistory.Get(0) - mHistory.Get(1); // divided by time elapsed
}