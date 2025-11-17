#pragma once
#include "../util/imports.h"

// aggregate initialization causes an ICE (internal compiler error)
// probably because of const vector + aggregate initialization
// to be fixed in Visual Studio 17.8

namespace Arc
{
    struct DebugCommand
    {
        // aggregate initialization causes an ICE
        DebugCommand(
            const std::string& name, const std::string& description, const std::string& usage
        ) : name(name), description(description), usage(usage) {}

        const std::string& name;
        const std::string& description;
        const std::string& usage;
    };

    inline const std::vector<DebugCommand> debugCommands = {
        DebugCommand("eval", "Evaluate an expression and get the result.", "eval <Code>"),
        DebugCommand("watch new", "Create a new watch on a variable or expression.", "watch new <Code> as <Watch Name>"),
        DebugCommand("watch get", "Get the value of a watch.", "watch get <Watch Name>"),
        DebugCommand("watch remove", "Stop watching a variable or expressions.", "watch remove <Watch Name>"),
        DebugCommand("watch all", "Get all watch values.", "watch all"),
        DebugCommand("continue", "Continue execution until the next breakpoint.", "continue"),
        DebugCommand("label new", "Label the current instruction to be able to jump to it later.", "label new <Label Name>"),
        DebugCommand("label remove", "Remove a label.", "label remove <Label Name>"),
        DebugCommand("label all", "Get all labels.", "label all"),
        DebugCommand("label jump", "Jump to a label.", "label jump <Label Name>"),
        DebugCommand("label marklog", "Mark a label as a logpoint.", "label marklog <Label Name> <Log>"),
        DebugCommand("timer startpoint", "Mark a label as a timer startpoint.", "timer startpoint <Timer Name> <Label Name>"),
        DebugCommand("timer endpoint", "Mark a label as a timer endpoint.", "timer endpoint <Timer Name> <Label Name>"),
        DebugCommand("timer start", "Start a timer.", "timer start <Timer Name>"),
        DebugCommand("timer stop", "Stop a timer.", "timer stop <Timer Name>"),
        DebugCommand("timer get", "Get the value of a timer.", "timer get <Timer Name>"),
        DebugCommand("timer reset", "Reset a timer.", "timer reset <Timer Name>"),
        DebugCommand("help", "Print help information on the debugger. If a command is specified, display help information about the command.", "help <Command>")
    };
}