#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "ClasslikeDeclaration.h"
#include "../../../util/imports.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct InterfaceDeclaration : ASTBodiedStatement, ASTModifiableStatement, ASTNamedStatement
    {
        InterfaceDeclaration(
            const ClasslikeDeclaration& classDeclaration
        ) : signature(*classDeclaration.signature), inheritanceSignatures(classDeclaration.inheritanceSignatures), modifiers(classDeclaration.modifiers),  constructorDeclarations(classDeclaration.constructorDeclarations), forwardDeclared(classDeclaration.forwardDeclared), body(classDeclaration.body), decorators(decorators) {}
        
        const Expression& signature;
        const std::vector<Expression>& inheritanceSignatures;
        const bool forwardDeclared;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::InterfaceDeclaration; }
    };
}