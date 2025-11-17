#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../../../util/Util.h"

namespace ASTNode
{
    struct ArrayAccessExpression : ASTExpression
    {
        ArrayAccessExpression(
            const Expression& accessee, const std::vector<Expression>& keys
        ) : accessee(accessee), keys(keys) {}
        
        const Expression& accessee;
        const std::vector<Expression>& keys;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::ArrayAccessExpression; }
    };
}