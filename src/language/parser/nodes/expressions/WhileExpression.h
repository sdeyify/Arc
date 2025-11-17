#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../../ExpressionKind.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"

namespace ASTNode
{
    struct WhileExpression : ASTBodiedExpression
    {
        WhileExpression(
            const Expression& condition, const Arc::AbstractSyntaxTree& body, const std::optional <Arc::AbstractSyntaxTree>& elseBody, std::vector<Modifier> modifiers
        ) : condition(condition), body(body), elseBody(elseBody), modifiers(modifiers) {}
    
        const Expression& condition;
        const Arc::AbstractSyntaxTree& body;
        const std::optional <Arc::AbstractSyntaxTree>& elseBody;
        std::vector<Modifier> modifiers;

        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::WhileExpression; }
    };
}