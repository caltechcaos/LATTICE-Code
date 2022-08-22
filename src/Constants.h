#pragma once

/**
 * Contains all global constants
 */

namespace lattice {
    constexpr double MAX_VOLTAGE = 3.3; // volts

    const int ANALOG_BITS = 12; // resolution
    constexpr double MAX_ANALOG = 4095; // arbitrary units

    const int SERIAL_RATE = 115200; // baud
}