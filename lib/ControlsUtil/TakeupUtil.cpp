#include "TakeupUtil.h"

#include <math.h>

#include <cmath>

static double Pi = 3.14159265358979324;
static double kMaxAngle = 109.0;
static double kMinAngle = 30.0;
static double kMinError = 0.01;

double lattice::DoubleArmTakeupAngles(double takeup, double armLength, double leaderPulleyX, double leaderPulleyY, double armX, double armY, double rPulley) {
    double smallestError = 100000.0;
    double targetAngle = 0.0;
    for (double i = kMinAngle; i <= kMaxAngle; i += 0.1) {  // Assumes takeup > 0
        double error = abs(takeup - GetTakeup(i, i, armLength, leaderPulleyX, leaderPulleyY, armX, armY, rPulley));
        if (error < smallestError) {
            targetAngle = i;
            smallestError = error;
        }
    }
    return targetAngle;
}

double lattice::SingleArmTakeupAngle(double takeup, double armLength, double leaderPulleyX, double leaderPulleyY, double armX, double armY, double rPulley) {
    double smallestError = 100000.0;
    double targetAngle = 0.0;
    for (double i = kMinAngle; i <= kMaxAngle; i += 0.1) {  // Asusmes takeup > 0
        double error = abs(takeup - GetTakeup(i, 0.0, armLength, leaderPulleyX, leaderPulleyY, armX, armY, rPulley));
        if (error < smallestError) {
            targetAngle = i;
            smallestError = error;
        }
    }
    return targetAngle;
}

double lattice::GetCorrespondingAngle(double otherAngle, double takeup, double armLength, double leaderPulleyX, double leaderPulleyY, double armX, double armY, double rPulley) {
    double smallestError = 100000.0;
    double targetAngle = 0.0;
    for (double i = 0.0; i <= kMaxAngle; i += 0.1) {
        double error = abs(takeup - GetTakeup(i, otherAngle, armLength, leaderPulleyX, leaderPulleyY, armX, armY, rPulley));
        if (error < kMinError) {
            return i;
        }
    }
    return targetAngle;
}

double lattice::SingleSideTakeup(double armAngle, double armLength, double leaderPulleyX, double leaderPulleyY, double armX, double armY, double rPulley) {
    if (armAngle >= kMinAngle) {
        armAngle *= Pi / 180.0;
        return -leaderPulleyX + rPulley * (2 * Pi - acos((2 * rPulley) / sqrt(pow(-armX + armLength * cos(armAngle), 2) + pow(-armY + armLength * sin(armAngle), 2))) - acos((2 * rPulley) / sqrt(pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2))) - acos((-pow(leaderPulleyX, 2) + pow(-armX + armLength * cos(armAngle), 2) + pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY + armLength * sin(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2)) / (2. * sqrt(pow(-armX + armLength * cos(armAngle), 2) + pow(-armY + armLength * sin(armAngle), 2)) * sqrt(pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2))))) + rPulley * (Pi - acos((2 * rPulley) / sqrt(pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2))) - asin((-armX + leaderPulleyX + armLength * cos(armAngle)) / sqrt(pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2)))) + 2 * sqrt(-pow(rPulley, 2) + (pow(-armX + armLength * cos(armAngle), 2) + pow(-armY + armLength * sin(armAngle), 2)) / 4.) + 2 * sqrt(-pow(rPulley, 2) + (pow(-armX + leaderPulleyX + armLength * cos(armAngle), 2) + pow(-armY - leaderPulleyY + armLength * sin(armAngle), 2)) / 4.);
    } else {
        return 0.0;
    }
}

double lattice::GetTakeup(double leftArmAngle, double rightArmAngle, double armLength, double leaderPulleyX, double leaderPulleyY, double armX, double armY, double rPulley) {
    return SingleSideTakeup(leftArmAngle, armLength, leaderPulleyX, leaderPulleyY, armX, armY, rPulley) + SingleSideTakeup(rightArmAngle, armLength, leaderPulleyX, leaderPulleyY, armX, armY, rPulley);
}