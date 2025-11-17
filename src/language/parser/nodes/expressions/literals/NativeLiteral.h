#pragma once
#include "ASTLiteral.h"
#include "../../../IRBitsizeKind.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct NativeLiteral : ASTLiteral
    {
        NativeLiteral(
            const Arc::IRBitsizeKind& bitsize, const std::string& value, const std::string& raw
        ) : bitsize(bitsize), value(value), raw(raw) {}

        const Arc::IRBitsizeKind& bitsize;
        const std::string& value;
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeLiteral; }
    };
}