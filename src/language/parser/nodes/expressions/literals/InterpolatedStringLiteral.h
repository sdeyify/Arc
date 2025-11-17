#pragma once
#include "ASTLiteral.h"
#include "../../Interpolation.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/Util.h"

namespace ASTNode
{
    struct InterpolatedStringLiteral : ASTLiteral
    {
        InterpolatedStringLiteral(
            const std::vector<StringInterpolation>& interpolations, const std::string& raw
        ) : interpolations(interpolations), raw(raw) {}

        const std::vector<StringInterpolation>& interpolations;
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::InterpolatedStringLiteral; }
    };
}