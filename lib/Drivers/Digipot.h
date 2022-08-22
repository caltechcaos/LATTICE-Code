#pragma once

namespace lattice {
    class Digipot {
        public:
        /**
         * Creates the two-channel digital potentiometers
         */
        Digipot();

        /**
         * Sets up the digipots for control
         */
        void Setup();

        /**
         * Sets the resistance of the first digipot
         * @param resistance Must be between 0-255, inclusive
         * @return True on success, false otherwise
         */
        bool Set1(int resistance);

        /**
         * Sets the resistance of the second digipot
         * @param resistance Must be between 0-255, inclusive
         * @return True on success, false otherwise
         */
        bool Set2(int resistance);

        /**
         * Sets the resistance of both digipots
         * @param resistance Must be between 0-255, inclusive
         * @return True on success, false otherwise
         */
        bool SetBoth(int resistance);

        private:
        /**
         * SPI-CS0 on the Due. Should not be changed.
         * Digipot should also be wired to MOSI, SCK pins
         * in the center of the board.
         */
        static const int SS_PIN = 10;

        /**
         * Sent over SPI to indicate which potentiometer to set the resistance of
         */
        static const int POT1 = 0x11;
        static const int POT2 = 0x12;
        static const int POT_BOTH = 0x13;

        bool SetPot(const int POT, int resistance);
    };
}