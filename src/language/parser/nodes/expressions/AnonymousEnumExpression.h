#pragma once
#include "ASTBodiedExpression.h"   
#include "../../AbstractSyntaxTree.h"   
#include "../Type.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"
#include "../statements/EnumDeclaration.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct AnonymousEnumExpression : ASTBodiedExpression
    {
        AnonymousEnumExpression(
            const std::unique_ptr<EnumDeclaration>& enumDeclaration
        ) : datatype(enumDeclaration->datatype), inheritanceSignatures(enumDeclaration->inheritanceSignatures), implementationSignatures(enumDeclaration->implementationSignatures),constructorDeclarations(enumDeclaration->constructorDeclarations), forwardDeclared(enumDeclaration->forwardDeclared), body(enumDeclaration->body), modifiers(enumDeclaration->modifiers), decorators(enumDeclaration->decorators) {}
        
        const Type& datatype;
        const std::vector<Expression>& inheritanceSignatures;
        const std::vector<Expression>& implementationSignatures;
        const bool forwardDeclared;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const Arc::AbstractSyntaxTree& constructorDeclarations;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;
    
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousEnumExpression; }
    };
}