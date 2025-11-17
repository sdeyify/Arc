#pragma once

namespace Arc
{
    namespace LogColors
    {
        inline const char* RESET = "\033[0m";
        inline const char* ERROR = "\033[31m";
        inline const char* WARNING = "\033[33m";
        inline const char* INFO = "\033[36m";
        inline const char* FATAL_ERROR = "\033[1;31m";
        inline const char* TRACE = "\033[1;32m";
        inline const char* DEBUG = "\033[1;35m";
    }
}