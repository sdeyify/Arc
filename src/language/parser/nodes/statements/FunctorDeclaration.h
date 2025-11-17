#pragma once
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../expressions/FunctorExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct FunctorDeclaration : ASTNamedStatement, ASTModifiableStatement
    {
        FunctorDeclaration(
            const std::shared_ptr<FunctorExpression>& functorExpression, const Expression& signature, std::vector<Modifier> modifiers
        ) : functorExpression(functorExpression), signature(signature), modifiers(modifiers) {}

        const std::shared_ptr<FunctorExpression>& functorExpression;
        const Expression& signature;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::FunctorDeclaration; }
    };
}