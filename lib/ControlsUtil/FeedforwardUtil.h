#pragma once

namespace lattice {
double GetSimpleFeedforward(double kS, double kV, double kA, double velocity, double desiredAcceleration);

double GetElevatorFeedforward(double kS, double kV, double kA, double kG, double velocity, double desiredAcceleration);

}  // namespace lattice