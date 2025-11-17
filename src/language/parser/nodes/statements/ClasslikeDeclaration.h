#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../Decorator.h"
#include "../Type.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ClasslikeDeclaration : ASTBodiedStatement, ASTModifiableStatement, ASTNamedStatement
    {
        ClasslikeDeclaration(
            const std::optional<Expression>& signature, const std::vector<Expression>& inheritanceSignatures, const std::vector<Expression>& implementationSignatures, const Arc::AbstractSyntaxTree& constructorDeclarations,
            const bool forwardDeclared, const std::optional <Arc::AbstractSyntaxTree>& body, const std::optional<Type>& datatype, const std::vector<Decorator>& decorators, std::vector<Modifier> modifiers
        ) : signature(signature), inheritanceSignatures(inheritanceSignatures), implementationSignatures(implementationSignatures), constructorDeclarations(constructorDeclarations), forwardDeclared(forwardDeclared), body(body), datatype(datatype), decorators(decorators), modifiers(modifiers) {}
        
        const std::optional<Expression>& signature;
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::optional<Type>& datatype;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ClassDeclaration; }
    };
}