#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ExpressionStatement : ASTStatement
    {
        ExpressionStatement(const Expression& expr) : expr(expr) {}

        const Expression& expr;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ExpressionStatement; }
    };
}