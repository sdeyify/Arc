#pragma once
#include "nodes/statements/ASTStatement.h"
#include "nodes/expressions/ASTExpression.h"
#include "nodes/types/ASTType.h"
#include "ASTIterationExitKind.h"
#include "../util/imports.h"

namespace Arc
{
    struct ASTIterator
    {
        ASTIterator(
            const Statement& stmt, const size_t& index
        ) : stmt(stmt), index(index) {}

        const Statement& stmt;
        const size_t& index;

        ASTIterationExitKind breakTraversal() const { return ASTIterationExitKind::BreakExit; }
        ASTIterationExitKind goToNextCondition() const { return ASTIterationExitKind::NextConditionExit; }
        ASTIterationExitKind continueTraversal() const { return ASTIterationExitKind::ContinueExit; }
        ASTIterationExitKind visitChildrenIfExists() const { return ASTIterationExitKind::VisitChildrenIfBodied; }

        template<std::derived_from<ASTNode::ASTStatement> T>
        T cast() const { return *std::dynamic_pointer_cast<T>(this->stmt); }
    };
}