#pragma once

namespace lattice {
class LimitSwitch {
   public:
    /**
     * Initializes the limit switch
     */
    LimitSwitch(const int pin);

    /**
     * Sets up to read a limit switch input from the given pin
     * (pulled high)
     */
    void Setup();

    /**
     * Updates the limit switch. Must be called EVERY loop cycle.
     */
    void Update();

    /**
     * Returns the value of the limit switch found at the last
     * update
     * @return If the limit switch is currently pushed
     */
    bool Get();

    /**
     * Returns whether the limit switch was newly pushed in this
     * update cycle
     * @return If the limit switch has been newly pushed down
     */
    bool Pushed();

    /**
     * Returns whether the limit switch was newly released in this
     * update cycle
     * @return If the limit switch has been newly released
     */
    bool Released();

   private:
    const int mPin;
    bool mState;
    bool mLast;
};
}  // namespace lattice