#pragma once
#include "ASTBodiedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ExtensionDeclaration : ASTBodiedStatement
    {
        ExtensionDeclaration(
            const std::vector<Expression>& extensionClasses, const std::vector<Expression>& interfaceSignatures, const Arc::AbstractSyntaxTree& body, std::vector<Modifier> modifiers
        ) : extensionClasses(extensionClasses), interfaceSignatures(interfaceSignatures), body(body), modifiers(modifiers) {}
        
        const std::vector<Expression>& extensionClasses;
        const std::vector<Expression>& interfaceSignatures;
        const Arc::AbstractSyntaxTree& body;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ExtensionDeclaration; }
    };
}