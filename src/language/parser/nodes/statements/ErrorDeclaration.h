#pragma once
#include "../ErrorDeclarationMember.h"
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ErrorDeclaration : ASTStatement
    {
        ErrorDeclaration(
            const std::vector<ErrorDeclarationMember>& members, const std::vector<Expression>& inheritanceSignatures
        ) : members(members), inheritanceSignatures(inheritanceSignatures) {}

        const std::vector<ErrorDeclarationMember>& members;
        const std::vector<Expression>& inheritanceSignatures;
        
        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ErrorDeclaration; }
    };
}