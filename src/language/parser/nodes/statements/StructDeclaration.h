#pragma once
#include "ASTBodiedStatement.h"
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "ClasslikeDeclaration.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct StructDeclaration : ASTBodiedStatement, ASTNamedStatement, ASTModifiableStatement
    {
        StructDeclaration(
            const ClasslikeDeclaration& classDeclaration
        ) : signature(*classDeclaration.signature), inheritanceSignatures(classDeclaration.inheritanceSignatures), modifiers(classDeclaration.modifiers), implementationSignatures(classDeclaration.implementationSignatures), constructorDeclarations(classDeclaration.constructorDeclarations), forwardDeclared(classDeclaration.forwardDeclared), body(classDeclaration.body), decorators(decorators) {}
        
        const Expression& signature;
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::StructDeclaration; }
    };
}