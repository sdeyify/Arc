#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct IteratorExpression : ASTExpression
    {
        IteratorExpression(
            const std::optional<Expression>& iterator, const Expression& iterable
        ) : iterator(iterator), iterable(iterable) {}
        
        const std::optional<Expression>& iterator;
        const Expression& iterable;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::IteratorExpression; }
    };
}