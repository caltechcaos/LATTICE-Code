#include "Util.h"

#include <Arduino.h>

#include "Constants.h"
//#include "Logger.h"

using namespace lattice;

void lattice::GenericSetup(std::string name) {
    analogWriteResolution(ANALOG_BITS);
    analogReadResolution(ANALOG_BITS);
    Serial.begin(SERIAL_RATE);
    //Logger::logger().Setup(name);
}