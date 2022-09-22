#pragma once

#include <Arduino.h>

#include "SpektrumSatellite.h"

namespace lattice {
/**
 * Class for interfacing with the spektrum satellite module.
 * Note that this module assumes that there is no trimming done on any of the controls.
 */
class RC {
   public:
    /**
     * Constructor for the RC Class
     *
     * @param comSerial The serial that the RC module is communicating with
     * @param rxPin The RX Pin the module is plugged into
     * @param powerPin The power pin the module is plugged into
     */
    RC(UARTClass& comSerial, int rxPin, int powerPin);

    /**
     * Sets up the RC to be in bind mode
     */
    void Setup();

    /**
     * Updates the RC. If its not binded, the LED will blink. If it is binded the LED will be solid.
     */
    void Update();

    /**
     * Gets the throttle value as a number in the range [-1, 1].
     * Note the throttle is the y-axis on the left joystick of the spektrum controller.
     *
     * @return The throttle position as a value from -1 to 1.
     */
    double GetThrottle();

    /**
     * Gets the aileron value as a number in the range [-1, 1].
     * Note the aileron is the x-axis on the right joystick of the spektrum controller.
     *
     * @return The aileron position as a value from -1 to 1.
     */
    double GetAileron();

    /**
     * Gets the elevator value as a number in the range [-1, 1].
     * Note the elevator is the y-axis on the right joystick of the spektrum controller.
     *
     * @return The elevator position as a value from -1 to 1.
     */
    double GetElevator();

    /**
     * Gets the rudder value as a number in the range [-1, 1].
     * Note the rudder is the x-axis on the left joystick of the spektrum controller.
     *
     * @return The rudder position as a value from -1 to 1.
     */
    double GetRudder();

    /**
     * Gets the gear value as an integer in the list [0, 1, 2].
     *
     * @return The gear value as either 0, 1, or 2.
     */
    int GetGear();

    /**
     * Gets the aux 1 value as an integer in the list [0, 1, 2].
     *
     * @return The aux 1 value as either 0, 1, or 2.
     */
    int GetAux1();

    /**
     * Gets the aux 2 value as an integer in the list [0, 1, 2].
     *
     * @return The aux 2 value as either 0, 1, or 2.
     */
    int GetAux2();

   private:
    int ProcessMinMidMaxInput(u_int16_t value);
    UARTClass& mComSerial;
    SpektrumSatellite<uint16_t> mSatellite;
    const int kRxPin;
    const int kPowerPin;
    bool mLEDState;
    bool mBinded = false;
    int mPrevBlink;

    /* Important constants */
    static constexpr int kLEDBlink = 1000;
    static constexpr u_int16_t kMaxThrottle = 1700;
    static constexpr u_int16_t kMidThrottle = 1025;
    static constexpr u_int16_t kMinThrottle = 342;
    static constexpr double kDeadband = 0.15;
    double ConvertOutput(double controllerInput);
};
}  // namespace lattice