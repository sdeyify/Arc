#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../../ExpressionKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../statements/ClassDeclaration.h"
#include "../statements/ASTStatement.h"

namespace ASTNode
{
    struct AnonymousClassExpression : ASTBodiedExpression
    {
        AnonymousClassExpression(
            const std::unique_ptr<ClassDeclaration>& classDeclaration
        ) : inheritanceSignatures(classDeclaration->inheritanceSignatures), implementationSignatures(classDeclaration->implementationSignatures), constructorDeclarations(classDeclaration->constructorDeclarations), forwardDeclared(classDeclaration->forwardDeclared), body(classDeclaration->body), modifiers(classDeclaration->modifiers), decorators(classDeclaration->decorators) {}
        
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;

        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::AnonymousClassExpression; }
    };
}