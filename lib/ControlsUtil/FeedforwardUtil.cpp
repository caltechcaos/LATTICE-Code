#include "FeedforwardUtil.h"

#include <math.h>
double lattice::GetSimpleFeedforward(double kS, double kV, double kA, double velocity, double accel) {
    return copysign(kS, velocity) + kV * velocity + kA * accel;
}

double lattice::GetElevatorFeedforward(double kS, double kV, double kA, double kG, double velocity, double accel) {
    return GetSimpleFeedforward(kS, kV, kA, velocity, accel) + kG;
}
