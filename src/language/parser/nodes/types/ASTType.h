#pragma once
#include "../../TypeKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ASTType : std::enable_shared_from_this<ASTType>
    {   
        virtual Arc::TypeKind kind() const = 0;

        template<std::derived_from<ASTType> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }
    };

    using Type = std::shared_ptr<ASTType>;
}

namespace Arc
{
    using Type = std::shared_ptr<ASTNode::ASTType>;
}