#pragma once

/**
 * Contains all generic utility functions for the main classes
 * See also: lib/Drivers/Constants.h
 */

namespace lattice {
    const int SERIAL_RATE = 115200; // baud
    const int ANALOG_BITS = 12; // resolution

    /**
     * Sets all parameters. Every main should call this.
     */
    void GenericSetup();
}