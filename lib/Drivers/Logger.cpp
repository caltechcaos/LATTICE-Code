#include "Logger.h"

using namespace lattice;

Logger::Logger() : serialMonitorThreshold(Priority::Verbose),
                   rcTelemetryThreshold(Priority::Warning) {}

bool Logger::Setup(std::string name) {
    if (name != "") {
        serialOut.print("===== ");
        serialOut.print(name.c_str());
        serialOut.println(" =====");
    }
    return true;
}

void Logger::SetThresholds(Priority serialMonitorThreshold,
                           Priority rcTelemetryThreshold) {
    Logger::serialMonitorThreshold = serialMonitorThreshold;
    Logger::rcTelemetryThreshold = rcTelemetryThreshold;
}

void Logger::Log(Priority priority, ErrorCode errorCode, std::string message) {
    message = "[" + std::to_string(static_cast<int>(errorCode)) + "] " +
        GetPriorityString(priority) + message;
    if (priority >= serialMonitorThreshold) {
        serialOut.println(message.c_str());
    }
    if (priority >= rcTelemetryThreshold) {
        // TODO: implement rc telemetry output
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