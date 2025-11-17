#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct NativeCall : ASTExpression
    {
        NativeCall(
            const Expression& expression, const std::vector<Expression>& arguments
        ) : expression(expression), arguments(arguments) {};

        const Expression& expression;
        const std::vector<Expression>& arguments;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeCall; }
    };
}