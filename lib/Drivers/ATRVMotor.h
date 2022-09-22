#pragma once
#include <Arduino.h>

namespace lattice {
/**
 * Class to interface with the motor controllers driving the ATRV.
 * Note: The pins for this motor controller NEED to be the arduino PWM pins, NOT the digital pins that are PWM enabled.
 */
class ATRVMotor {
   public:
    /**
     * Constructor for the ATRV Class.
     *
     * @param forwardPWMPin Pin that commands the forward PWM
     * @param backwardPWMPin Pin that commands the backward PWM
     */
    ATRVMotor(int forwardPWMPin, int backwardPWMPin);

    /**
     * Sets up the motor controller
     */
    void Setup();

    /**
     * Runs the motor controller at a specific percent output.
     */
    void SetPower(double power);

   private:
    void SetPWM(int forwardPWM, int backwardPWM);
    const int kForwardPWMPin;
    const int kBackwardPWMPin;
};
}  // namespace lattice
