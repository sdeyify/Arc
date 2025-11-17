#pragma once
#include "ASTLiteral.h"
#include "../../../IRBitsizeKind.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct CustomSizedNativeLiteral : ASTLiteral
    {
        CustomSizedNativeLiteral(
            const bool isSigned, const uint8_t bitsize, const std::string& value, const std::string& raw
        ) : raw(raw), bitsize(bitsize), value(value), isSigned(isSigned) {}

        const bool isSigned;
        const uint8_t bitsize;
        const std::string& value;
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::CustomSizedNativeLiteral; }
    };
}