#pragma once
#include "ErrorId.h"
#include "Log.h"
#include "LogKind.h"
#include "../util/imports.h"

namespace Arc
{
    struct ErrorMessage : virtual Log
    {
        ErrorMessage(
            const std::string& message, const ErrorId& id
        ) : message(message), id(id) {}

        const std::string& message;
        const ErrorId& id;

        LogKind kind() const override { return LogKind::ErrorLog; }
    };
}