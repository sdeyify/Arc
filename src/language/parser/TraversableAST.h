#pragma once
#include "../util/imports.h"
#include "AbstractSyntaxTree.h"
#include "ASTIterationExitKind.h"
#include "ASTIterationCommand.h"
#include "StatementKind.h"

namespace Arc
{
    class TraversableAST
    {
    public:
        TraversableAST(const AbstractSyntaxTree& ast) : ast(ast) {}
    private:
        const AbstractSyntaxTree& ast;
        std::vector<ASTIterationCommand> commands;
    public:
        TraversableAST& doFn(const std::function<ASTIterationExitKind(const ASTIterator&)>& fn);
        TraversableAST& doIf(const std::function<bool(const ASTIterator&)>& condition, const std::function<ASTIterationExitKind(const ASTIterator&)>& fn);
        TraversableAST& ifOfKind(const StatementKind& kind, const std::function<ASTIterationExitKind(const ASTIterator&)>& fn);

        TraversableAST copyFor(const AbstractSyntaxTree& otherAst) const;

        void traverse();
    };
}