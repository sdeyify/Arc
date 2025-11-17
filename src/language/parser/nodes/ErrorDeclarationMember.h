#pragma once
#include "../../util/imports.h"

namespace ASTNode
{
    struct ErrorDeclarationMember
    {
            ErrorDeclarationMember(const Expression& signature, const std::optional<Expression>& message) : signature(signature), message(message) {}
            Expression signature;
        std::optional<Expression> message;
    };
}