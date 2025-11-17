#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ContinueStatement : ASTStatement
    {
        ContinueStatement(const Expression& expression) : expression(expression) {}

        const Expression& expression;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ContinueStatement; }
    };
}