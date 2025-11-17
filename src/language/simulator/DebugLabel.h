#pragma once
#include "DebugTimerLink.h"
#include "../util/imports.h"

namespace Arc
{
    struct DebugLabel
    {
        DebugLabel(const size_t instructionPtr) : instructionPtr(instructionPtr) {}

        std::optional<std::string> logMessage;
        size_t instructionPtr;
        std::optional<DebugTimerLink> timerLink;
    };
}