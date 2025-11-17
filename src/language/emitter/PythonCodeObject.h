#pragma once
#include "PythonCodeObjectFlag.h"
#include "../util/Util.h"

namespace Arc
{
    struct PythonCodeObject
    {
        size_t argcount = 0;
        size_t posonlyargcount = 0;
        size_t kwonlyargcount = 0;
        size_t nlocals = 0;
        size_t stacksize = 0;
        std::vector<PythonCodeObjectFlag> flags;
        std::vector<byte> code;
        std::vector<std::vector<byte>> constants;
        std::vector<std::string> names;
        std::string filename = "<unknown>";
        std::string name = "<module>";
        std::string qualifyingName = "<module>";
        size_t firstLineNumber = 1;
        std::map<size_t, size_t> lineTable;  
    };
}