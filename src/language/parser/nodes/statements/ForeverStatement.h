#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../../../util/Util.h"
#include "../../StatementKind.h"

namespace ASTNode
{
    struct ForeverStatement : ASTBodiedStatement, ASTModifiableStatement
    {
        ForeverStatement(
            const Arc::AbstractSyntaxTree& body, std::vector<Modifier> modifiers
        ) : body(body), modifiers(modifiers) {}

        const Arc::AbstractSyntaxTree& body;
        std::vector<Modifier> modifiers;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::ForeverStatement; }
    };
}