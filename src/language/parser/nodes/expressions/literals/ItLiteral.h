#pragma once
#include "ASTLiteral.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct ItLiteral : ASTLiteral
    {
        ItLiteral() : raw("it") {}

        const std::string& raw;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ItLiteral; }
    };
}