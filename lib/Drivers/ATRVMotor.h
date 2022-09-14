#pragma once
#include <Arduino.h>

namespace lattice{

class ATRVMotor{
    // public:
    // /**
    //  * Creates a button output on the given pin
    //  * @param PIN Which pin to use as an output
    //  */
    // ATRVMotor(const int LeftPin, const int RightPin);

    // /**
    //  * Sets up to use the button output. Must be called before
    //  * using it.
    //  */
    // void Setup();
    // /**
    //  * Sets the button output. It will stay that value until
    //  * it's set again.
    //  * @param value True for HIGH, false for LOW
    //  */
    // void Set(bool value);
        
    // private:
    //  const int LeftM;
    //  const int RightM;
    //  const int EN = 8;

    // };

    public:

	    ATRVMotor(uint8_t EN, uint8_t L_PWM, uint8_t R_PWM) : ATRVMotor(EN, 0, L_PWM, R_PWM) {}

        ATRVMotor(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM);

        void Enable();
        void Disable();

        void SetPWM(uint8_t LeftPWM, uint8_t RightPWM);

	    // void TurnLeft(uint8_t pwm);
	    // void TurnRight(uint8_t pwm);
        // void Forward(u_int8_t pwm);

	    void Stop();

    private:
        uint8_t _L_EN;
        uint8_t _R_EN;
        uint8_t _L_PWM;
        uint8_t _R_PWM;

    };
}
