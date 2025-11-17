#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"

namespace ASTNode
{
    struct NativeFunctionType : ASTType
    {
        Arc::TypeKind kind() const override { return Arc::TypeKind::NativeFunctionType; }
    };
}