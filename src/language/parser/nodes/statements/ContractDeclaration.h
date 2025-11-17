#pragma once
#include "ASTNamedStatement.h"
#include "ASTModifiableStatement.h"
#include "../Modifier.h"
#include "../expressions/ContractExpression.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ContractDeclaration : ASTNamedStatement, ASTModifiableStatement
    {
        ContractDeclaration(
            const Expression& signature, std::vector<Modifier> modifiers, const ContractExpression& contractExpr
        ) : signature(signature), modifiers(modifiers), contractExpr(contractExpr) {}

        const Expression& signature;
        std::vector<Modifier> modifiers;
        const ContractExpression& contractExpr;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ContractDeclaration; }
    };
}