#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../Type.h"
#include "../Generic.h"
#include "../Parameter.h"
#include "../Modifier.h"
#include "../statements/FunctionDeclaration.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct AnonymousFunctionExpression : ASTBodiedExpression
    {
        AnonymousFunctionExpression(
            const std::unique_ptr<FunctionDeclaration>& fnDeclaration
        ) : datatype(fnDeclaration->datatype), generics(fnDeclaration->generics), selectedSymbols(fnDeclaration->selectedSymbols), parameters(fnDeclaration->parameters), forwardDeclared(fnDeclaration->forwardDeclared), body(*fnDeclaration->body), decorators(fnDeclaration->decorators), modifiers(fnDeclaration->modifiers), satisfiers(fnDeclaration->satisfiers) {}

        const Type& datatype;
        const std::vector<Generic>& generics;
        const std::vector<Expression>& selectedSymbols;
        const std::vector<Parameter>& parameters;
        const bool forwardDeclared;
        const std::vector<Decorator>& decorators;
        const Arc::AbstractSyntaxTree& body;
        const std::vector<Expression> satisfiers;
        std::vector<Modifier> modifiers;
        
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousFunctionExpression; }
    };
}