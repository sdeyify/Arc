#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct WhenStatement : ASTBodiedStatement, ASTModifiableStatement
    {
        WhenStatement(
            const Expression& condition, const Arc::AbstractSyntaxTree& body, std::vector<Modifier> modifiers
        ) : condition(condition), body(body), modifiers(modifiers) {}    

        const Expression& condition;
        const Arc::AbstractSyntaxTree& body;
        std::vector<Modifier> modifiers;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::WhenStatement; }
    };
}