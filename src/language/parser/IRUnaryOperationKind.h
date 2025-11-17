#pragma once

namespace Arc
{
    enum class IRUnaryOperationKind
    {
        Not,
        Negative,
        Positive,
        Ceil,
        Floor,
        CountLeadingZeros,
        CountEndingZeros,
        CountLeadingOnes,
        CountEndingOnes,
        CountZeros,
        CountOnes
    };
}