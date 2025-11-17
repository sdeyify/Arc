#pragma once
#include "../../util/imports.h"
#include "Type.h"
#include "Parameter.h"

namespace ASTNode
{
    struct Generic
    {
        Generic(
            const bool span, const bool optional, const bool named, const std::string& signature, const Type& defaultValue, const std::vector<Generic>& generics, const std::vector<Parameter>& parameters
        ) : span(span), optional(optional), named(named), signature(signature), defaultValue(defaultValue), generics(generics), parameters(parameters) {}
        
        const bool span;
        const bool optional;
        const bool named;
        const std::string& signature;
        const Type& defaultValue;
        const std::vector<Generic>& generics;
        const std::vector<Parameter>& parameters;
    };
}