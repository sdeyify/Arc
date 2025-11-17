#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct IfStatement : ASTBodiedStatement, ASTModifiableStatement
    {
        IfStatement(
            const Expression& condition, const Arc::AbstractSyntaxTree& body, const Arc::AbstractSyntaxTree& elseBody, std::vector<Modifier> modifiers
        ) : condition(condition), body(body), elseBody(elseBody), modifiers(modifiers) {}

        const Expression& condition;
        const Arc::AbstractSyntaxTree& body;
        const Arc::AbstractSyntaxTree& elseBody;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::IfStatement; }
    };
}