#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"

namespace ASTNode
{
    struct InferType : ASTType
    {
        Arc::TypeKind kind() const override { return Arc::TypeKind::InferType; }
    };
}