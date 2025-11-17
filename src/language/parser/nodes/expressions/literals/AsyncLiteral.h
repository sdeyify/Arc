#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"
#include "ASTLiteral.h"

namespace ASTNode
{
    struct AsyncLiteral : ASTLiteral
    {
        AsyncLiteral() : raw("async") {}
    
        const std::string& raw;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AsyncLiteral; }
    };
}