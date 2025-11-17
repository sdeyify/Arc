#pragma once
#include "BreakpointEnvironment.h"
#include "StackValueKind.h"
#include "../util/imports.h"

namespace Arc
{
    struct SimulationEnvironment
    {
        const std::map<size_t, std::string>& structNames;
        const std::map<size_t, std::string>& functionNames;
        const std::map<size_t, StackValueKind>& pointerDirectiveValueKinds;
        const std::map<std::string, std::string>& environmentVariables;
        const BreakpointEnvironment& breakpointEnvironment;
        const std::string& path;
    };
}