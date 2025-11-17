#pragma once

namespace Arc
{
    enum class ASTIterationExitKind
    {
        ContinueExit,
        NextConditionExit,
        BreakExit,
        VisitChildrenIfBodied
    };
}