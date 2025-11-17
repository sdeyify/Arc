#include "Messages.h"
#include "../util/imports.h"
#include <iostream>
#include <functional>

void Arc::ArcLogger::writeln(const std::string& message, const LogLevel& logLevel) {
    std::string prefix;
    std::string color;

    switch (logLevel) {
        case Arc::LogLevel::Error: prefix = "[ERROR]"; color = Arc::LogColors::ERROR; break;
        case Arc::LogLevel::Info: prefix = "[INFO]"; color = Arc::LogColors::INFO; break;
        case Arc::LogLevel::Warning: prefix = "[WARNING]"; color = Arc::LogColors::WARNING; break;
        case Arc::LogLevel::FatalError: prefix = "[FATAL]"; color = Arc::LogColors::FATAL_ERROR; break;
        case Arc::LogLevel::Trace: prefix = "[DEBUB]"; color = Arc::LogColors::TRACE; break;
        case Arc::LogLevel::Debug: prefix = "[DEBUB]"; color = Arc::LogColors::DEBUG; break;
    }
    
    std::cout << color + prefix + " " + message + Arc::LogColors::RESET << std::endl;

    Arc::ArcLogger::logs.push_back(Arc::LoggedMessage(message, logLevel));
}

void Arc::ArcLogger::storeLog(const std::string& message, const LogLevel& logLevel) {
    Arc::ArcLogger::logs.push_back(Arc::LoggedMessage(message, logLevel));
    Arc::ArcLogger::storedLogs.push_back(Arc::LoggedMessage(message, logLevel));
}

void Arc::ArcLogger::flushStoredLogs() {
    for (const Arc::LoggedMessage& log : Arc::ArcLogger::storedLogs) {
        Arc::ArcLogger::writeln(log.message, log.logLevel);
    }
}