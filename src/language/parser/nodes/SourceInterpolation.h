#pragma once
#include "expressions/literals/StringLiteral.h"
#include "InterpolationKind.h"
#include "Interpolation.h"

namespace ASTNode
{
    struct SourceInterpolation : Interpolation
    {
        SourceInterpolation(const StringLiteral& value) : value(value) {}

        const StringLiteral& value;
        InterpolationKind kind() const override { return InterpolationKind::SourceInterpolation; }
    };
}