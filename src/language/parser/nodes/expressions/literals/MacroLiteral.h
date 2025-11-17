#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct MacroLiteral : ASTLiteral
    {
        MacroLiteral() : raw("macro") {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::MacroLiteral; }
    };
}