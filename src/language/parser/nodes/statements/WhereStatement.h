#pragma once
#include "ASTBodiedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct WhereStatement : ASTBodiedStatement
    {
        WhereStatement(
            const Expression& condition, const Arc::AbstractSyntaxTree& body
        ) : condition(condition), body(body) {}
        
        const Expression& condition;
        const Arc::AbstractSyntaxTree& body;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::WhereStatement; }
    };
}