#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct SuperLiteral : ASTLiteral
    {
        SuperLiteral() : raw("super") {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::StringLiteral; }
    };
}