#pragma once
#include <Stepper.h>

namespace lattice {
    class HandoffMotor {
        public:
        /**
         * Sets up to control the handoff motor off the given pins
         * @param steps The number of steps per revolution in the handoff motor
         * @param pin1, pin2, pin3, pin4 The four pins connected to the motor
         */
        HandoffMotor(const int steps, const int pin1, const int pin2,
            const int pin3, const int pin4);

        /**
         * Sets the speed of the handoff motor
         * @param speed Speed in rpm
         * @return True if successful, false if error
         */
        bool SetSpeed(long speed);

        /**
         * Instructs the handoff motor to advance the given
         * number of steps
         * @param steps The steps to move: positive for forwards, negative
         *      for reverse
         */
        void Move(int steps);

        private:
        Stepper mMotor;
    };
}