#pragma once
#include "ASTLiteral.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct ContractLiteral : ASTLiteral
    {
        ContractLiteral() : raw("contract") {}

        const std::string& raw;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ContractLiteral; }
    };
}