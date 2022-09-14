#pragma once
//#include <Stepper.h>
#include <Arduino.h>
#include "DRV8834.h"

namespace lattice {
    class HandoffMotor {
        public:
        /**
         * Sets up to control the handoff motor off the given pins
         * @param StepPin and @param DriverPin
         */
        HandoffMotor(const int StepPin, const int DriverPin,
            const int StepPerRev, const int kRPM, const int kMicrosteps);

        /**
        * Sets up for control of the elevator motor. Must be called before using it
        */
        void Setup();

        /**
         * 
         * Sets the speed of the handoff motor
         * @param speed Speed in rpm
         * @return True if successful, false if error
         */
        void SetAccel(int MotorAccel, int MotorDecel);

        void SetMicroStep(int mode);

        /**
         * Instructs the handoff motor to advance the given
         * number of steps
         * @param steps The steps to move: positive for forwards, negative
         *      for reverse
         * @return True on successful movement, false otherwise
         */
        void StepMove(int steps);

        /**
         * Instructs the handoff motor to advance the given
         * number of steps
         * @param steps The steps to move: positive for forwards, negative
         *      for reverse
         * @return True on successful movement, false otherwise
         */
        void MicroStepMove(int steps);

        /**
         * Sets the brake to the given value, overriding any movement
         * @param brake True to stop, false to go
         * @return True on success, false otherwise
         */
        //bool SetBrake(bool brake);

        private:
        //static const int STEPS_PER_REV = 200;
        // DRV8834::DRV8834(short steps, short dir_pin, short step_pin)
        DRV8834 mMotor;
        const int RPM;
        const int MicroSteps;
        int Mode = 1;
        // const int StepsPRev;
        //bool brake;
    };
}