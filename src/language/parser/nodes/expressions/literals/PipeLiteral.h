#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct PipeLiteral : ASTLiteral
    {
        PipeLiteral() : raw("pipe") {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::PipeLiteral; }
    };
}