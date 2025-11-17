#pragma once
#include "ASTBodiedStatement.h"
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct MacroDeclaration : ASTBodiedStatement, ASTNamedStatement, ASTModifiableStatement
    {
        MacroDeclaration(
            const Expression& signature, bool forwardDeclared, const std::optional <Arc::AbstractSyntaxTree>& body, std::vector<Modifier> modifiers
        ) : signature(signature), forwardDeclared(forwardDeclared), body(body), modifiers(modifiers) {}
        
        const Expression& signature;
        const bool forwardDeclared;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::MacroDeclaration; }
    };
}