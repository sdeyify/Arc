#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../Type.h"

namespace ASTNode
{
    struct CallExpression : ASTExpression
    {
        CallExpression(
            const std::vector<Expression>& arguments, const Expression& callee, const std::vector<Type>& generics
        ) : arguments(arguments), callee(callee), generics(generics) {}
        
        const Expression& callee;
        const std::vector<Type>& generics;
        const std::vector<Expression>& arguments;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::CallExpression; }
    };
}