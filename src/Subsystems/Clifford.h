#pragma once
#include "ButtonOut.h"
#include "Digipot.h"

namespace lattice {
/**
 * A class for controlling Clifford.
 *
 * Assumes the digipot is wired to digital 10, MOSI, and SCK as required, with
 * pot 1 as joystick x, and pot 2 as joystick y.
 */
class Clifford {
   public:
    /**
     * Creates controls for Clifford
     * @param kButtonPin The pin connected to the joystick back trigger input
     */
    Clifford(const int kButtonPin);

    /**
     * Sets up Clifford for control. Must be called before other use.
     */
    void Setup();

    /**
     * Provides the given joystick input to Clifford. It will remain
     * at that value until new ones are sent.
     * @param x The joystick x value input, between 0 and 1, inclusive
     * @param y The joystick y value input, between 0 and 1, inclusive
     * @return True on successful movement, false otherwise
     */
    bool Move(double x, double y);

    /**
     * Sets the brake to the given value, overriding any movement
     * @param brake True to stop, false to go
     * @return True on success, false otherwise
     */
    bool SetBrake(bool brake);

   private:
    bool brake;
    Digipot joystick;
    ButtonOut trigger;

    /**
     * Sets the joystick to its zero position
     */
    void Zero();
};
}  // namespace lattice