#pragma once
#include "../../../util/imports.h"
#include "../../TypeKind.h"
#include "../expressions/ASTExpression.h"
#include "ASTType.h"

namespace ASTNode
{
    struct ObjectType : ASTType
    {
        ObjectType(const std::optional<Expression>& whereCondition) : whereCondition(whereCondition) {};

        const std::optional<Expression>& whereCondition;

        Arc::TypeKind kind() const override { return Arc::TypeKind::ObjectType; };
    };
}