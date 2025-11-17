#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct SpreadExpression : ASTExpression
    {
        SpreadExpression(const Expression& spread) : spread(spread) {}

        const Expression& spread;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::SpreadExpression; }
    };
}