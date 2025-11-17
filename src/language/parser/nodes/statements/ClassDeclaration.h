#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "ClasslikeDeclaration.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ClassDeclaration : ASTBodiedStatement, ASTModifiableStatement, ASTNamedStatement
    {
        ClassDeclaration(
            const ClasslikeDeclaration& classDeclaration
        ) : signature(*classDeclaration.signature), inheritanceSignatures(classDeclaration.inheritanceSignatures), implementationSignatures(classDeclaration.implementationSignatures), constructorDeclarations(classDeclaration.constructorDeclarations), forwardDeclared(classDeclaration.forwardDeclared), body(classDeclaration.body), modifiers(classDeclaration.modifiers), decorators(classDeclaration.decorators) {}
    
        const Expression& signature;
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const std::optional<Arc::AbstractSyntaxTree>& body;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ClassDeclaration; }
    };
}