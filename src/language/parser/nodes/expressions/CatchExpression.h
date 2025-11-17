#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct CatchExpression : ASTExpression
    {
        CatchExpression(
            const Expression& error, const std::optional<Expression>& alias, const std::optional<Expression>& whereCondition
        ) : error(error), alias(alias), whereCondition(whereCondition) {}
        
        const Expression& error;
        const std::optional<Expression>& alias;
        const std::optional<Expression>& whereCondition;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::CatchExpression; }
    };
}