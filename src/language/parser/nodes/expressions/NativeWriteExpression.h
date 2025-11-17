#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "literals/StringLiteral.h"

namespace ASTNode
{
    struct NativeWriteExpression : ASTExpression
    {
        NativeWriteExpression(const StringLiteral& message) : message(message) {}

        const StringLiteral& message;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeWriteExpression; }
    };
}