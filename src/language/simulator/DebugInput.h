#pragma once
#include "DebugCommand.h"
#include "../util/imports.h"
#include "../util/split.h"
#include "../util/string_util.h"

namespace Arc
{
    class DebugInput
    {
    public:
        static inline DebugInput fromString(const std::string& input) {
            DebugInput debugInput;
            debugInput.arguments = Util::split(input, std::vector(Util::WHITESPACE.begin(), Util::WHITESPACE.end()));
            return debugInput;
        }
    private:
        std::vector<std::string> arguments;
    public:
        inline bool empty() const { return this->arguments.empty(); }

        inline std::string at(const int index) const {
            return (index >= 0)
                ? this->arguments[index]
                : this->arguments[this->arguments.size() + index];
        }

        inline std::string next() {
            std::string command = this->at(0);
            this->arguments.erase(this->arguments.begin());
            return command;
        }

        inline std::string expect(const std::string& errorMessage) {
            if (this->empty()) {
                // throw err
                std::unreachable();
            }

            return this->next();
        }
    };

    inline DebugInput debugInput;
}