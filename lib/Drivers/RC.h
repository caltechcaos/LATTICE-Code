#pragma once

#include <Arduino.h>

#include "SpektrumSatellite.h"

namespace lattice {
/**
 * Class for interfacing with the spektrum satellite module.
 */
class RC {
   public:
    /**
     * Constructor for the RC Class
     *
     * @param comSerial The serial that the RC module is communicating with
     * @param logSerial The serial to log text information to
     * @param rxPin The RX Pin the module is plugged into
     * @param powerPin The power pin the module is plugged into
     * @param onboardLEDPin The pin of the onboard LED
     */
    RC(UARTClass comSerial, UARTClass logSerial, int rxPin, int powerPin, int onboardLEDPin);

    /**
     * Sets up the RC to be in bind mode
     */
    void Setup();

    /**
     * Updates the RC. If its not binded, the LED will blink. If it is binded the LED will be solid.
     */
    void Update();

    /**
     * Gets the throttle value as a number in the range [0, 1].
     */
    double GetThrottle();

    /**
     * Gets the Aileron value as an integer 0, 1, 2
     */
    int GetAileron();

    /**
     * Gets the Elevator value as an integer 0, 1, 2
     */
    int GetElevator();

    /**
     * Gets the Rudder value as an integer 0, 1, 2
     */
    int GetRudder();

    /**
     * Gets the gear value as an integer (don't know the output format).
     */
    int GetGear();

    /**
     * Gets the Aux1 value as an integer (don't know the output format).
     */
    int GetAux1();

    /**
     * Gets the Aux2 value as an integer (don't know the output format).
     */
    int GetAux2();

   private:
    int ProcessMinMidMaxInput(u_int16_t value);
    UARTClass mLogSerial;
    UARTClass mComSerial;
    SpektrumSatellite<uint16_t> mSatellite;
    const int kRxPin;
    const int kPowerPin;
    const int kOnboardLEDPin;
    bool mLEDState;
    int mPrevBlink;

    /* Important constants */
    static constexpr int kLEDBlink = 1000;
    static constexpr u_int16_t kMaxThrottle = 1700;
    static constexpr u_int16_t kMidThrottle = 1025;
    static constexpr u_int16_t kMinThrottle = 342;
};
}  // namespace lattice