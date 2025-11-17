#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct AnonymousNamespaceExpression : ASTBodiedExpression
    {
        AnonymousNamespaceExpression(const Arc::AbstractSyntaxTree& body) : body(body) {}

        const Arc::AbstractSyntaxTree& body;

        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousNamespaceExpression; }
    };
}