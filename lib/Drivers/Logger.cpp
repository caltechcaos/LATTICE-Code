#include "Logger.h"

using namespace lattice;

Logger::Logger() : serialMonitorThreshold(Priority::Log),
                   rcTelemetryThreshold(Priority::Warning),
                   fileThreshold(Priority::Verbose) {}

void Logger::SetThresholds(Priority serialMonitorThreshold,
                           Priority rcTelemetryThreshold, Priority fileThreshold) {
    Logger::serialMonitorThreshold = serialMonitorThreshold;
    Logger::rcTelemetryThreshold = rcTelemetryThreshold;
    Logger::fileThreshold = fileThreshold;
}

void Logger::Log(Priority priority, size_t messageCode, std::string message) {
    message = GetPriorityString(priority) + "[" + std::to_string(messageCode) + "] " + message;
    if (priority >= serialMonitorThreshold) {
        serialOut.print(message.c_str());
    }
    if (priority >= fileThreshold) {
        //
    }
    if (priority >= rcTelemetryThreshold) {
        //
    }
}

std::string Logger::GetPriorityString(Priority priority) {
    switch (priority) {
        case Priority::Error:
            return "ERROR: ";
        case Priority::Warning:
            return "Warning: ";
        case Priority::Log:
        case Priority::Verbose:
        default:
            return "";
    }
}