#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../expressions/IteratorExpression.h"
#include "../Modifier.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ForeachStatement : ASTBodiedStatement, ASTModifiableStatement
    {
        ForeachStatement(
            const Arc::AbstractSyntaxTree& body, const std::shared_ptr<IteratorExpression>& iterator, std::vector<Modifier> modifiers
        ) : body(body), iterator(iterator), modifiers(modifiers) {}
        
        const Arc::AbstractSyntaxTree& body;
        const std::shared_ptr<IteratorExpression>& iterator;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ForeachStatement; }
    };
}