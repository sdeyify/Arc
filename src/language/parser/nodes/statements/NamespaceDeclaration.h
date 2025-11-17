#pragma once
#include "ASTBodiedStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct NamespaceDeclaration : ASTBodiedStatement, ASTNamedStatement
    {
        NamespaceDeclaration(
            const Expression& name, const Arc::AbstractSyntaxTree& body
        ) : name(name), body(body) {}

        const Expression& name;
        const Arc::AbstractSyntaxTree& body;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::NamespaceDeclaration; }
    };
}