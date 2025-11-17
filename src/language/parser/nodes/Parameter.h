#pragma once
#include "../../util/imports.h"
#include "Type.h"
#include "expressions/ASTExpression.h"

namespace ASTNode
{
    struct Parameter
    {
        Parameter(
            const bool span, const bool needed, const bool named, const bool constant, const bool referencePassed, const std::optional<Expression>& defaultVal, const std::string& signature, const Type& datatype
        ) : span(span), needed(needed), named(named), constant(constant), referencePassed(referencePassed), defaultValue(defaultVal), signature(signature), datatype(datatype) {}
        
        const bool span;
        const bool needed;
        const bool named;
        const bool constant;
        const bool referencePassed;
        const std::optional<Expression>& defaultValue;
        const std::string& signature;
        const Type& datatype;
    };
}