#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"
#include "../expressions/ASTExpression.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ThrowType : ASTType
    {
        ThrowType(
            const Type& type, const std::vector<Expression>& errors
        ) : type(type), errors(errors) {}

        const Type& type;
        const std::vector<Expression>& errors;

        Arc::TypeKind kind() const override { return Arc::TypeKind::ThrowType; }
    };
}