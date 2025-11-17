#pragma once
#include "../../util/imports.h"
#include "Type.h"
#include "expressions/ASTExpression.h"

namespace ASTNode
{
    struct Decorator
    {
        Decorator(
            const Expression& signature, const std::vector<Type>& generics, const std::vector<Expression>& arguments
        ) : signature(signature), generics(generics), arguments(arguments) {}
        
        const Expression& signature;
        const std::vector<Type>& generics;
        const std::vector<Expression>& arguments;
    };
}