#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct ArrayLiteral : ASTLiteral
    {
        ArrayLiteral(
            const std::string& raw, const std::vector<Expression>& values
        ) : raw(raw), values(values) {}

        const std::string& raw;
        const std::vector<Expression>& values;
        
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ArrayLiteral; }
    };
}