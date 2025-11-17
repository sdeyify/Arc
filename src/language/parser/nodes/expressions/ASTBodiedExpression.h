#pragma once
#include "ASTExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ASTBodiedExpression : virtual ASTExpression
    {
        ASTBodiedExpression();

        virtual Arc::ExpressionKind kind() const = 0;
        virtual bool hasBody() const override { return true; }
        virtual bool isLiteral() const override { return false; }
        
        const Arc::AbstractSyntaxTree& body;
    };

    using BodiedExpression = std::shared_ptr<ASTBodiedExpression>;
}

namespace Arc
{
    using BodiedExpression = std::shared_ptr<ASTNode::ASTBodiedExpression>;
}