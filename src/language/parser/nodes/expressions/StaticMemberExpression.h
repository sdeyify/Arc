#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct StaticMemberExpression : ASTExpression
    {
        StaticMemberExpression(
            const Expression& owner, const Expression& member
        ) : owner(owner), member(member) {}
        
        const Expression& owner;
        const Expression& member;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::StaticMemberExpression; }
    };
}