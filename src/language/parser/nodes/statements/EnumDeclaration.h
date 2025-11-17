#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../Type.h"
#include "../../../util/imports.h"
#include "ClasslikeDeclaration.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct EnumDeclaration : ASTBodiedStatement, ASTModifiableStatement, ASTNamedStatement
    {
        EnumDeclaration(
            const ClasslikeDeclaration& classDeclaration
        ) : signature(*classDeclaration.signature), inheritanceSignatures(classDeclaration.inheritanceSignatures), implementationSignatures(classDeclaration.implementationSignatures), constructorDeclarations(classDeclaration.constructorDeclarations), datatype(*classDeclaration.datatype), forwardDeclared(classDeclaration.forwardDeclared), body(classDeclaration.body), decorators(decorators), modifiers(classDeclaration.modifiers) {}
    
        const Expression& signature;
        const Type& datatype;
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const std::vector<Decorator> decorators;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::EnumDeclaration; }
    };
}