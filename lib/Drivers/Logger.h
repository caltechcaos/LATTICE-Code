#pragma once
#include <Arduino.h>

#undef min
#undef max
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
        Verbose,    // Not necessary for normal operation
        Log,        // Normal operation
        Warning,    // Shouldn't occur, but not a deal-breaker
        Error       // Code cannot meaningfully continue after this
    };

    /**
     * Codes for errors, for display on RC telemetry output
     * Higher numbers should be assigned to those of higher priority
     */
    enum class ErrorCode {
        Message = 0
    };

    /**
     * Sets up to use this logger. Must be called before logging to it.
     * Prefer to use a name whenever possible.
     * @param name The name of the current program. Appended to the
     *      start of the new log file
     * @returns True on success, false otherwise
     */
    bool Setup(std::string name = "");

    /**
     * Sets the thresholds at which point something should
     * appear in the respective outputs, inclusive
     * @param serialMonitorThreshold Threshold for Serial Monitor and file (default: Verbose)
     * @param rcTelemetryThreshold Threshold for RC telemetry (default: Warning)
     */
    void SetThresholds(Priority serialMonitorThreshold, Priority rcTelemetryThreshold);

    /**
     * Writes something to the logs.
     * @param priority The priority to write it with
     * @param errorCode The code for this message, to be displayed on RC telemetry
     * @param message The message to log
     */
    void Log(Priority priority, ErrorCode errorCode, std::string message);

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