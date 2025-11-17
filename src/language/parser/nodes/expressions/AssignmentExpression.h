#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct AssignmentExpression : ASTExpression
    {
        AssignmentExpression(
            const Expression& left, const Expression& right, const std::string& op
        ) : left(left), right(right), op(op) {}
        
        const Expression& left;
        const Expression& right;
        const std::string& op;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AssignmentExpression; }
    };   
}