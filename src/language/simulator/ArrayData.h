#pragma once
#include "../parser/IRBitsizeKind.h"

namespace Arc
{
    struct ArrayData
    {
        ArrayData(
            const size_t pointerIdx, const size_t length, const IRBitsizeKind& bitsize
        ) : pointerIdx(pointerIdx), length(length), bitsize(bitsize) {}

        const size_t pointerIdx;
        const size_t length;
        const IRBitsizeKind& bitsize;
    };
}