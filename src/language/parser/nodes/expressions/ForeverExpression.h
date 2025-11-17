#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"
#include "../../StatementKind.h"
#include "../../../util/Util.h"

namespace ASTNode
{
    struct ForeverExpression : ASTBodiedExpression
    {
        ForeverExpression(
            const Arc::AbstractSyntaxTree& body, std::vector<Modifier> modifiers
        ) : body(body), modifiers(modifiers) {}

        const Arc::AbstractSyntaxTree& body;
        std::vector<Modifier> modifiers;

        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ForeverExpression; }
    };
}