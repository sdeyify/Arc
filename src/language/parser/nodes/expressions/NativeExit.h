#pragma once
#include "ASTExpression.h"
#include "literals/NumericLiteral.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct NativeExit : ASTExpression
    {
        NativeExit(const NumericLiteral& exitCode) : exitCode(exitCode) {}

        const NumericLiteral& exitCode;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeExit; }
    };
}