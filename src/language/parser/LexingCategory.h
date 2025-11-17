#pragma once

namespace Arc
{
    enum class LexingCategory {
        Unset,
        Numeric,
        Alphanumeric,
        String,
        Character,
        InlineComment,
        MultilineComment,
        Symbol
    };
}