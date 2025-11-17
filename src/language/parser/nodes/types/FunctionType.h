#pragma once
#include "ASTType.h"
#include "../Parameter.h"
#include "../Generic.h"
#include "../expressions/ASTExpression.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct FunctionType : ASTType
    {
        FunctionType(
            const std::vector<Generic>& generics, const std::vector<Parameter>& parameters, const std::vector<Expression>& selectedSymbols, const Type& returnType
        ) : generics(generics), parameters(parameters), selectedSymbols(selectedSymbols), returnType(returnType) {}

        const std::vector<Generic>& generics;
        const std::vector<Parameter>& parameters;
        const std::vector<Expression>& selectedSymbols;
        const Type& returnType;
        
        Arc::TypeKind kind() const override { return Arc::TypeKind::FunctionType; }
    };
}