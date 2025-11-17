#pragma once
#include "../../../util/imports.h"
#include "../../StatementKind.h"

namespace ASTNode
{
    struct ASTStatement : std::enable_shared_from_this<ASTStatement>
    {
        virtual Arc::StatementKind kind() const = 0;
        virtual bool hasBody() const = 0;
        virtual bool hasModifiers() const = 0;
        virtual bool hasName() const = 0;
        
        template<std::derived_from<ASTStatement> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }
    };

    using Statement = std::shared_ptr<ASTStatement>;
}

namespace Arc
{
    using Statement = std::shared_ptr<ASTNode::ASTStatement>;
}