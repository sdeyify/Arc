#pragma once
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"
#include "../Parameter.h"
#include "../Generic.h"
#include "../Type.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct TypeDeclaration : ASTNamedStatement, ASTModifiableStatement
    {
        TypeDeclaration(
            const Expression& signature, const std::vector<Generic>& generics, const std::vector<Parameter>& parameters, bool forwardDeclared, const std::optional<Type>& value, const std::vector<Decorator>& decorators, const std::vector<Modifier> modifiers
        ) : signature(signature), generics(generics), parameters(parameters), forwardDeclared(forwardDeclared), value(value), decorators(decorators), modifiers(modifiers) {}
        
        const Expression& signature;
        const std::vector<Generic>& generics;
        const std::vector<Parameter>& parameters;
        const bool forwardDeclared;
        const std::vector<Decorator>& decorators;
        const std::optional<Type>& value;
        std::vector<Modifier> modifiers;
        
        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::TypeDeclaration; }
    };
}