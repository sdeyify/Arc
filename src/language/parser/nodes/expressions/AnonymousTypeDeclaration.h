#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../statements/TypeDeclaration.h"

namespace ASTNode
{
    struct AnonymousTypeDeclaration : ASTExpression
    {
        AnonymousTypeDeclaration(
            const std::unique_ptr<TypeDeclaration>& typeDeclaration
        ) : signature(typeDeclaration->signature), generics(typeDeclaration->generics), parameters(typeDeclaration->parameters), forwardDeclared(typeDeclaration->forwardDeclared), value(typeDeclaration->value) {}
        
        const Expression& signature;
        const std::vector<Generic>& generics;
        const std::vector<Parameter>& parameters;
        const bool forwardDeclared;
        const std::optional<Type>& value;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousTypeDeclaration; }
    };
}