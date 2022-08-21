#pragma once

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
     * @param mLambdaf A feed-forward function of setpoint
     */
    PIDF(const double kP, const double kI, const double kD,
         const std::function<double(double)> mLambdaf);

    /**
     * Constructs a PIDF controller with constant feed-forward
     * @param kP Proportional constant
     * @param kI Integral constant
     * @param kD Derivative constant
     * @param kF Feedforward constant
     */
    PIDF(const double kP, const double kI, const double kD, const double kF);

    /**
     * Executes this PIDF controller on a given state and returns the inputs
     * for the actuators
     * @param feedback The feedback sensor's value, representing current state
     * @param setpoint The target state
     * @return A tuple: input for actuator; true if controller was run successfully,
     *      false if there was an error.
     */
    std::tuple<double, bool> Run(double feedback, double setpoint);

   private:
    const double kP;
    const double kI;
    const double kD;
    const std::function<double(double)> mLambdaf;

    static const size_t HISTORY_SIZE = 2;
    CircularBuffer<double, HISTORY_SIZE> mHistory;
    double mAccum;

    /**
     * Calculates the integral of all past points. Requires adding to the history first.
     * @return The integral
     */
    double CalcI();

    /**
     * Calculates the derivative of the past HISTORY_SIZE points. Strongly recommend
     * adding to the history first.
     * @return The derivative
     */
    double CalcD();
};
}  // namespace lattice