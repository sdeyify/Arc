#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ReturnStatement : ASTStatement
    {
        ReturnStatement(const std::optional<Expression>& expr) : expr(expr) {}
        
        const std::optional<Expression>& expr;
        
        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ReturnStatement; }
    };
}