#pragma once
#include "ASTBodiedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ModifierApplyStatement : ASTBodiedStatement
    {
        ModifierApplyStatement(
            const std::vector<Modifier>& appliedModifers, const Arc::AbstractSyntaxTree& body
        ) : appliedModifers(appliedModifers), body(body) {}
        
        const std::vector<Modifier>& appliedModifers;
        const Arc::AbstractSyntaxTree& body;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ModifierApplyStatement; }
    };
}