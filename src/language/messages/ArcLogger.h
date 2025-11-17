#pragma once
#include "LogLevel.h"
#include "../util/imports.h"
#include "LoggedMessage.h"

namespace Arc
{
    class ArcLogger
    {
    private:
        inline static std::vector<LoggedMessage> logs;
        inline static std::vector<LoggedMessage> storedLogs;
    public:
        static void storeLog(const std::string& message, const LogLevel& logLevel);
        static void flushStoredLogs();

        static void writeln(const std::string& message, const LogLevel& logLevel);

        static void infoln(const std::string& message) { ArcLogger::writeln(message, LogLevel::Info); }
        static void warnln(const std::string& message) { ArcLogger::writeln(message, LogLevel::Warning); }
        static void errln(const std::string& message) { ArcLogger::writeln(message, LogLevel::Error); }
        static void traceln(const std::string& message) { ArcLogger::writeln(message, LogLevel::Trace); }
        static void dbgln(const std::string& message) { ArcLogger::writeln(message, LogLevel::Debug); }
        static void fatal_errln(const std::string& message) { ArcLogger::writeln(message, LogLevel::FatalError); }
    };
}