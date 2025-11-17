#pragma once
#include "ASTType.h"
#include "../expressions/ASTExpression.h"
#include "../../TypeKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct MergedType : ASTType
    {
        MergedType(
            const std::vector<Type>& types, const std::optional<Expression>& whereCondition
        ) : types(types), whereCondition(whereCondition) {}

        const std::vector<Type>& types;
        const std::optional<Expression>& whereCondition;

        Arc::TypeKind kind() const override { return Arc::TypeKind::MergedType; }
    };
}