#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct BinaryExpression : ASTExpression
    {
        BinaryExpression(
            const Expression& left, const Expression& right, const std::string& op
        ) : right(right), left(left), op(op) {}
        
        const Expression& left;
        const Expression& right;
        const std::string& op;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::BinaryExpression; }
    };
}