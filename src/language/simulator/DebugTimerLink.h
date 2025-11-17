#pragma once
#include "DebugTimerPoint.h"
#include "../util/imports.h"

namespace Arc
{
    struct DebugTimerLink
    {
        DebugTimerLink(
            const DebugTimerPoint& point, const std::string& timerName
        ) : point(point), timerName(timerName) {}

        DebugTimerPoint point;
        std::string timerName;
    };
}