#pragma once
#include "../util/imports.h"
#include "nodes/Statement.h"
#include "ASTIterator.h"
#include "AbstractSyntaxTree.h"

namespace Arc
{
    class ASTMatcher
    {
    public:
        ASTMatcher(const AbstractSyntaxTree& ast) : astStream(ast.getASTStream()) {};
    private:
        const std::vector<Arc::Statement>& astStream;
    public:
        std::optional<Statement> matchFirst(const std::function<bool(const Arc::ASTIterator& iterator)>& fn, const bool checkChildren) const;
        std::vector<Statement> matchAll(const std::function<bool(const Arc::ASTIterator& iterator)>& fn, const bool checkChildren) const;
    };
}