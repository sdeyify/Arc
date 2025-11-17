#pragma once

namespace Arc
{
    enum class StackValueKind
    {
        Void,
        Null,
        Bitvalue,
        Function,
        Array,
        Pointer,
        Object
    };
}