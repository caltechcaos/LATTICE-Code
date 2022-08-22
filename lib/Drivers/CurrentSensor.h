#pragma once

namespace lattice {
    class CurrentSensor {
        public:
        /**
         * Creates a current sensor on the given analog pin
         */
        CurrentSensor(const int PIN);
    };
}