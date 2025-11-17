#pragma once
#include "../../../util/imports.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct ASTExpression : std::enable_shared_from_this<ASTExpression>
    {
        virtual Arc::ExpressionKind kind() const = 0;
        virtual bool hasBody() const = 0;
        virtual bool isLiteral() const = 0;

        template<std::derived_from<ASTExpression> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }
    };

    using Expression = std::shared_ptr<ASTExpression>;
}

namespace Arc
{
    using Expression = std::shared_ptr<ASTNode::ASTExpression>;
}