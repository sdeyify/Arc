#pragma once
#include "../util/imports.h"
#include "ASTIterationExitKind.h"
#include "ASTIterator.h"

namespace Arc
{
    struct ASTIterationCommand
    {
        ASTIterationCommand(
            const std::function<ASTIterationExitKind(const ASTIterator&)>& fn,
            const std::function<bool(const ASTIterator&)>& condition = [](const ASTIterator& iterator) { return true; }
        ) : condition(condition), fn(fn) {}

        const std::function<bool(const ASTIterator&)>& condition;
        const std::function<ASTIterationExitKind(const ASTIterator&)>& fn;
    };
}