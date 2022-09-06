#pragma once

namespace lattice {
class ButtonOut {
   public:
    /**
     * Creates a button output on the given pin
     * @param PIN Which pin to use as an output
     */
    ButtonOut(const int PIN);

    /**
     * Sets up to use the button output. Must be called before
     * using it.
     */
    void Setup();

    /**
     * Sets the button output. It will stay that value until
     * it's set again.
     * @param value True for HIGH, false for LOW
     */
    void Set(bool value);

   private:
    const int M_PIN;
};
}  // namespace lattice