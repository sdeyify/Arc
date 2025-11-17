#pragma once

namespace Util
{
    enum class Compiler {
        MSVC,
        Clang,
        GCC,
        Unknown
    };

    inline Compiler getCompiler() {
        #if defined(_MSC_VER)
            return Compiler::MSVC;
        #elif defined(__clang__)
            return Compiler::Clang;
        #elif defined(__GNUC__)
            return Compiler::GCC;
        #else
            return Compiler::Unknown;
        #endif
    }
}