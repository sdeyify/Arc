#pragma once
#include "../util/imports.h"
#include "nodes/statements/ASTStatement.h"

namespace Arc
{
    class TraversableAST;
    class ASTMatcher;

    class AbstractSyntaxTree
    {
    public:
        AbstractSyntaxTree(const std::vector<ASTNode::Statement>& astStream) : astStream(astStream) {}
        AbstractSyntaxTree() : astStream(std::vector<ASTNode::Statement>()) {}
    private:
        std::vector<ASTNode::Statement> astStream;
    public:
        ASTNode::Statement at(const int index) const;
        std::vector<ASTNode::Statement> getASTStream() const;
        bool empty() const;
        size_t size() const;

        TraversableAST traverse() const;
        ASTMatcher matcher() const;
    };
}