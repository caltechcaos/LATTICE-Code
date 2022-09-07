#pragma once

#undef max
#undef min
#include <array>
#include <functional>
#include <tuple>

#include "CircularBuffer.h"

namespace lattice {
/**
 * Contains a PIDF controller which is initialized with PIDF constants
 * (or an optional feedforward function) and can be subsequently
 * executed on arbitrary inputs.
 */
class PIDF {
   public:
    /**
     * Constructs a PIDF controller with variable feed-forward
     * @param kP Proportional constant
     * @param kI Integral constant
     * @param kD Derivative constant
     * @param kError Max Error Threshold
     * @param mLambdaf A feed-forward function of setpoint
     * @param period The period the controller runs at in seconds
     */
    PIDF(const double kP, const double kI, const double kD,
         const std::function<double(double)> mLambdaf, double errorThreshold = 0.1, double derivThreshold = 0.1, double period = 0.02);

    /**
     * Constructs a PIDF controller with constant feed-forward
     * @param kP Proportional constant
     * @param kI Integral constant
     * @param kD Derivative constant
     * @param kError Max Error Threshold
     * @param kF Feedforward constant
     * @param period The period the controller runs at in seconds
     */
    PIDF(const double kP, const double kI, const double kD, const double kF, double errorThreshold = 0.1, double derivThreshold = 0.1, double period = 0.02);

    /**
     * Executes this PIDF controller on a given state and returns the inputs
     * for the actuators
     * @param feedback The feedback sensor's value, representing current state
     * @param setpoint The target state
     * @return Input for actuator.
     */
    double Run(double feedback, double setpoint);
    /**
     * Executes the PIDF controller on a given state and returns input for actuators
     *
     * @param feedback The feedback sensor's value, representing current state
     * @return Input for actuator.
     */
    double Run(double feedback);

    /**
     * Returns if we're at our target. Note, this should be run after a call to Run in a loop.
     *
     * @return If our error and derivative of state are within our defined tolerances.
     */
    bool AtTarget();

    /**
     * Updates the setpoint
     */
    void SetTarget(double setpoint);

    /**
     * Calculates the integral of all past points. Requires adding to the history first.
     * Note, this function should not be called, its only exposed for testing purposes.
     * @return The integral
     */
    double CalcI();

    /**
     * Calculates the derivative of the past HISTORY_SIZE points. Strongly recommend
     * adding to the history first.
     * Note, this function should not be called, its only exposed for testing purposes.
     * @return The derivative
     */
    double CalcD();

    double GetSetpoint() { return mSetpoint; };

   private:
    const double kP;
    const double kI;
    const double kD;
    const std::function<double(double)> mLambdaf;

    static const size_t HISTORY_SIZE = 3;
    CircularBuffer<double, HISTORY_SIZE> mHistory;
    double mAccum = 0.0;
    double mSetpoint = 0.0;
    const double kPeriod;
    const double kMaxError;
    const double kMaxDerivError;
};
}  // namespace lattice