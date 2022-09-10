#pragma once

namespace lattice{
/* 
 * The brake will go through a relay because the arduino does not have enough power
 * the code will funtion like a button only functioning high and low
 */
class Brake{
    public:
    /**
     * Creates a button output on the given pin
     * @param PIN Which pin to use as an output
     */
    Brake(const int PIN);

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
}