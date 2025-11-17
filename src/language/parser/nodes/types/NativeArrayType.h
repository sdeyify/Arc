#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct NativeArrayType : ASTType
    {
        NativeArrayType(const std::vector<Type>& types) : types(types) {};

        const std::vector<Type>& types;

        Arc::TypeKind kind() const override { return Arc::TypeKind::NativeArray; }
    };
}