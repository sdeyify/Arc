#pragma once
#include "../../../util/imports.h"
#include "../../TypeKind.h"
#include "../expressions/ASTExpression.h"
#include "ASTType.h"

namespace ASTNode
{
    struct BasicType : ASTType
    {
        BasicType(
            const Expression& signature, const std::vector<Type>& generics, const std::vector<Expression>& parameters, const std::optional<Expression>& whereCondition
        ) : signature(signature), generics(generics), parameters(parameters), whereCondition(whereCondition) {}
        
        const Expression& signature;
        const std::vector<Type>& generics;
        const std::vector<Expression>& parameters;
        const std::optional<Expression>& whereCondition;

        Arc::TypeKind kind() const override { return Arc::TypeKind::BasicType; }
    };
}