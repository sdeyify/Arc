#pragma once
#include "expressions/ASTExpression.h"
#include "InterpolationKind.h"
#include "Interpolation.h"

namespace ASTNode
{
    struct ExpressionInterpolation : Interpolation
    {
        ExpressionInterpolation(const Expression& value) : value(value) {}
        const Expression value;
        InterpolationKind kind() const override { return InterpolationKind::ExpressionInterpolation; }
    };
}