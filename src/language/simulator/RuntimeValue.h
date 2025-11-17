#pragma once
#include "StackValueKind.h"
#include "../util/imports.h"
#include "../../../../dependencies/half.hpp"
#include "../../../../dependencies/bfloat16.hpp"

namespace Arc
{
    using RuntimeInternalValue = std::variant<
        std::monostate, bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, half_float::half, bfloat16, double_t, float_t
    >;
    
    template<typename T>
    concept runtime_value_v = std::is_convertible_v<T, std::intmax_t> || std::is_convertible_v<T, float> || std::is_same_v<T, std::monostate>;

    template<typename T>
    concept float_runtime_value_v = std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, half_float::half> || std::is_same_v<T, bfloat16>;
    
    struct RuntimeValue
    {
        RuntimeValue(
            RuntimeInternalValue value, StackValueKind valueKind
        ) : value(value), valueKind(valueKind) {}

        RuntimeInternalValue value;
        StackValueKind valueKind;
    };
}