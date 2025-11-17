#pragma once
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"
#include "../../TypeKind.h"
#include "ASTType.h"

namespace ASTNode
{
    struct TypeofType : ASTType
    {
        TypeofType(
            const Expression& expr, const std::optional<Expression>& whereCondition
        ) : expr(expr), whereCondition(whereCondition) {}

        const Expression& expr;
        const std::optional<Expression>& whereCondition;
        
        Arc::TypeKind kind() const override { return Arc::TypeKind::TypeofType; };
    };
}