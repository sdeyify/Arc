#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct UnaryExpression : ASTExpression
    {
        UnaryExpression(
            const Expression& expr, bool postfix, const std::string& op
        ) : expr(expr), postfix(postfix), op(op) {}
        
        const Expression& expr;
        const bool postfix;
        const std::string& op;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::UnaryExpression; }
    };
}