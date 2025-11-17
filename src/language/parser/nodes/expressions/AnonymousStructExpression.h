#pragma once
#include "../../AbstractSyntaxTree.h"
#include "ASTBodiedExpression.h"
#include "../Modifier.h"
#include "../statements/StructDeclaration.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct AnonymousStructExpression : ASTBodiedExpression
    {
        AnonymousStructExpression(
            const std::unique_ptr<StructDeclaration>& structDeclaration
        ) : inheritanceSignatures(structDeclaration->inheritanceSignatures), implementationSignatures(structDeclaration->implementationSignatures),constructorDeclarations(structDeclaration->constructorDeclarations), forwardDeclared(structDeclaration->forwardDeclared), body(structDeclaration->body), modifiers(structDeclaration->modifiers), decorators(structDeclaration->decorators) {}
        
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;
 
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousStructExpression; }
    };
}