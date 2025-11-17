#pragma once
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"
#include "../Modifier.h"
#include "../Type.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct VariableDeclaration : ASTNamedStatement, ASTModifiableStatement
    {
        VariableDeclaration(
            bool constant, const Expression& signature, const std::optional<Type>& datatype, bool forwardDeclared, const std::optional<Expression>& value, const std::vector<Decorator>& decorators, std::vector<Modifier> modifiers
        ) : constant(constant), signature(signature), datatype(datatype), forwardDeclared(forwardDeclared), value(value), decorators(decorators), modifiers(modifiers) {}
        
        const bool constant;
        const Expression& signature;
        const std::optional<Type>& datatype;
        const bool forwardDeclared;
        const std::optional<Expression>& value;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;
 
        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::VariableDeclaration; }
    };
}