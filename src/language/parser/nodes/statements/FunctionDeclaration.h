#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Type.h"
#include "../Generic.h"
#include "../Parameter.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct FunctionDeclaration : ASTBodiedStatement, ASTModifiableStatement, ASTNamedStatement
    {
        FunctionDeclaration(
            const Expression& signature, const Type& datatype, const std::vector<Generic>& generics, const std::vector<Expression>& selectedSymbols, const std::vector<Modifier> modifiers, const std::vector<Parameter>& parameters, bool forwardDeclared, const std::optional <Arc::AbstractSyntaxTree>& body, const std::vector<Decorator>& decorators, const std::vector<Expression> satisfiers
        ) : signature(signature), datatype(datatype), generics(generics), parameters(parameters), forwardDeclared(forwardDeclared), selectedSymbols(selectedSymbols), body(body), modifiers(modifiers), decorators(decorators), satisfiers(satisfiers) {}
        
        const Expression& signature;
        const Type& datatype;
        const std::vector<Generic>& generics;
        const std::vector<Expression>& selectedSymbols;
        const std::vector<Parameter>& parameters;
        const bool forwardDeclared;
        const std::vector<Decorator>& decorators;
        const std::vector<Expression> satisfiers;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::FunctionDeclaration; }
    };
}