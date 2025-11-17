#pragma once
#include "ASTLiteral.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct NativeArrayLiteral : ASTLiteral
    {
        NativeArrayLiteral(
            const std::string& raw, const std::vector<Expression>& values
        ) : raw(raw), values(values) {};

        const std::string& raw;
        const std::vector<Expression>& values;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeArrayLiteral; }
    };
}