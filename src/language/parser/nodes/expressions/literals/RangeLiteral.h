#pragma once
#include "ASTLiteral.h"
#include "../ASTExpression.h"
#include "../../../ExpressionKind.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct RangeLiteral : ASTLiteral
    {
        RangeLiteral(const std::string& raw, const Expression& start, const Expression& end) : raw(raw), start(start), end(end) {}
        
        const std::string& raw;
        const Expression& start;
        const Expression& end;

        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::RangeLiteral; }
    };
}