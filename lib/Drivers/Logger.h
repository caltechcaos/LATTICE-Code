#pragma once
#include <Arduino.h>

#include <string>

namespace lattice {
/**
 * A logger class (singleton) which writes to the computer's Serial Monitor,
 * the RC's telemetry (if we figure out how to), and a logging file at varying
 * levels of priority.
 */
class Logger {
   public:
    /**
     * Singleton to ensure we only have one logger subsystem floating around
     * @returns The single instance of the logger
     */
    static Logger& logger() {
        static Logger logger;
        return logger;
    }

    /**
     * Priority levels for each message logged
     */
    enum class Priority {
        Verbose,
        Log,
        Warning,
        Error
    };

    /**
     * Sets the thresholds at which point something should
     * appear in the respective outputs, inclusive
     * @param serialMonitorThreshold Threshold for Serial Monitor (default: Log)
     * @param rcTelemetryThreshold Threshold for RC telemetry (default: Warning)
     * @param fileThreshold Threshold for file logging (default: Verbose)
     */
    void SetThresholds(Priority serialMonitorThreshold, Priority rcTelemetryThreshold,
                       Priority fileThreshold);

    /**
     * Writes something to the logs.
     * @param priority The priority to write it with
     * @param messageCode The code for this message, to be displayed on RC telemetry
     * @param message The message to log
     */
    void Log(Priority priority, size_t messageCode, std::string message);

    // Guarantee the singleton
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;

   private:
    /**
     * Creates the logger. Must be setup later before use
     */
    Logger();

    // At what priority level should something be logged in each
    Priority serialMonitorThreshold;
    Priority rcTelemetryThreshold;
    Priority fileThreshold;

    // The Serial Monitor output to print to. Don't change unless
    // you have very good reason to
    UARTClass& serialOut = Serial;

    /**
     * Converts a priority into a string
     * @param priority The priority to convert
     * @return The associated standardized flag for that priority
     */
    std::string GetPriorityString(Priority priority);
};
}  // namespace lattice