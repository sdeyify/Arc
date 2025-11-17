#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"
#include "../expressions/ASTExpression.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct OptionalType : ASTType
    {
        OptionalType(const Type& type) : type(type) {}

        const Type& type;

        Arc::TypeKind kind() const override { return Arc::TypeKind::ThrowType; }
    };
}