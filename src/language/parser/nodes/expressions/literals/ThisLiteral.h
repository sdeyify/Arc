#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct ThisLiteral : ASTLiteral
    {
        ThisLiteral() : raw("this") {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::StringLiteral; }
    };
}