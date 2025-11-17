#pragma once
#include "../../../ExpressionKind.h"
#include "../ASTExpression.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct ASTLiteral : virtual ASTExpression
    {
        ASTLiteral();

        virtual Arc::ExpressionKind kind() const = 0;
        
        const std::string& raw;
        
        virtual bool isLiteral() const override { return true; }
        virtual bool hasBody() const override { return false; }
    };

    using Literal = std::shared_ptr<ASTLiteral>;
}

namespace Arc
{
    using Literal = std::shared_ptr<ASTNode::ASTLiteral>;
}