#pragma once

namespace lattice {
/*
 * The estop triggers a relay to turn on or cut off the main power to the shuttle.
 */
class EStop {
   public:
    /**
     * Creates a button output on the given pin
     * @param PIN Which pin to use as an output
     */
    EStop(const int PIN);

    /**
     * Sets up to use the button output. Must be called before
     * using it.
     */
    void Setup();
    /**
     * Disables the robot
     */
    void Disable();

    /**
     * Enables the robot
     */
    void Enable();

   private:
    const int M_PIN;

    void Set(bool value);
};
}  // namespace lattice