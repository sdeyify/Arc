#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct ThrowExpression : ASTExpression
    {
        ThrowExpression(const Expression& expr) : expr(expr) {}
        
        const Expression& expr;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ThrowExpression; }
    };
}