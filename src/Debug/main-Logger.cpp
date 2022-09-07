#include "Logger.h"
#include "Util.h"

void setup() {
    lattice::GenericSetup("Logger Test");
}

void loop() {
    typedef lattice::Logger::Priority P;
    typedef lattice::Logger::ErrorCode E;
    lattice::Logger &logger = lattice::Logger::logger();

    logger.Log(P::Verbose, E::Message, "verbose");
    logger.Log(P::Log, E::Message, "Log.");
    logger.Log(P::Warning, E::Message, "Warning");
    logger.Log(P::Error, E::Message, "Error!!");
    delay(10000); // 10 seconds
}