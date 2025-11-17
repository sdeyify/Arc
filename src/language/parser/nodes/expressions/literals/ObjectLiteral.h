#pragma once
#include "ASTLiteral.h"
#include "../../statements/ASTStatement.h"
#include "../../Modifier.h"
#include "../../../ExpressionKind.h"
#include "../../../AbstractSyntaxTree.h"
#include "../../../../util/imports.h"

namespace ASTNode
{
    struct ObjectLiteral : ASTLiteral
    {
        ObjectLiteral(
            const Arc::AbstractSyntaxTree& entries, std::vector<Modifier> modifiers
        ) : entries(entries), modifiers(modifiers) {}
        
        const Arc::AbstractSyntaxTree& entries;
        std::vector<Modifier> modifiers;

        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ObjectLiteral; }
    };
}