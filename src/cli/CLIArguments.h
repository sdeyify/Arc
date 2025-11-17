#pragma once
#include "imports.h"

namespace ArcCLI
{
    struct CLIArguments
    {
        int count;
        std::vector<std::string> arguments;
        bool initialized = false;

        std::string at(const int index) const {
            return (index >= 0)
                ? arguments[index]
                : arguments[arguments.size() + index];
        }

        std::string next() {
            const std::string argument = arguments.front();
            arguments.erase(arguments.begin());
            return argument;
        }

        bool empty() const {
            return this->arguments.empty();
        }
    };

    inline CLIArguments cliArguments;
}