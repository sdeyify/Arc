#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct AssignmentStatement : ASTStatement
    {
        AssignmentStatement(
            const Expression& assigneeSignature, const Expression& assignedValue, const std::string& op
        ) : assigneeSignature(assigneeSignature), assignedValue(assignedValue), op(op) {}

        const Expression& assigneeSignature;
        const Expression& assignedValue;
        const std::string& op;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::AssignmentStatement; }
    };
}