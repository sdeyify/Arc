#pragma once
#include "Log.h"
#include "LogKind.h"
#include "LogLevel.h"
#include "../util/imports.h"

namespace Arc
{
    struct LoggedMessage : virtual Log
    {
        LoggedMessage(
            const std::string& message, const LogLevel& logLevel
        ) : message(message), logLevel(logLevel) {}

        const std::string& message;
        const LogLevel& logLevel;

        LogKind kind() const override { return LogKind::MessagedLog; }
    };
}