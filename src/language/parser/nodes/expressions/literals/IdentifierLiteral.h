#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct IdentifierLiteral : ASTLiteral
    {
        IdentifierLiteral(const std::string& raw) : raw(raw) {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::IdentifierLiteral; }
    };
}