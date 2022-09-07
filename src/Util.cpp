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
double lattice::remap(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
