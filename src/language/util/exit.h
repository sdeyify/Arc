#pragma once
#include "imports.h"
#include <iostream>

namespace Util
{
    inline void quit(const int code = EXIT_FAILURE) {
        exit(code);
    }
}