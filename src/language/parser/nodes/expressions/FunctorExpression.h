#pragma once
#include "ASTExpression.h"
#include "../Type.h"
#include "../Modifier.h"
#include "../Parameter.h"
#include "../Generic.h"
#include "../Decorator.h"
#include "../statements/FunctionDeclaration.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct FunctorExpression : ASTExpression
    {
        FunctorExpression(
            const FunctionDeclaration& fnDecl
        ) : generics(fnDecl.generics), parameters(fnDecl.parameters), selectedSymbols(fnDecl.selectedSymbols), decorators(fnDecl.decorators), modifiers(fnDecl.modifiers), satisfiers(fnDecl.satisfiers), returnType(fnDecl.datatype) {}

        const std::vector<Generic>& generics;
        const std::vector<Parameter>& parameters;
        const std::vector<Expression>& selectedSymbols;
        const std::vector<Decorator>& decorators;
        const std::vector<Modifier>& modifiers;
        const std::vector<Expression>& satisfiers;
        const Type& returnType;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::FunctorExpression; }
    };
}