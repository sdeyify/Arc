#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../Parameter.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct NativeFunction : ASTBodiedExpression
    {
        NativeFunction(
            const Arc::AbstractSyntaxTree& body, const std::vector<Parameter>& parameters
        ) : body(body), parameters(parameters) {};

        const Arc::AbstractSyntaxTree& body;
        const std::vector<Parameter>& parameters;

        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::NativeFunction; }
    };
}