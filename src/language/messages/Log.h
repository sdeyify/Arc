#pragma once
#include "LogKind.h"
#include "../util/imports.h"

namespace Arc
{
    struct Log
    {
        Log() : message("") {}

        const std::string& message;
        virtual LogKind kind() const = 0;
    };
}