#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct IfExpression : ASTBodiedExpression
    {
        IfExpression(
            const Expression& condition, const Arc::AbstractSyntaxTree& body, const Arc::AbstractSyntaxTree& elseBody, std::vector<Modifier> modifiers
        ) : condition(condition), body(body), elseBody(elseBody), modifiers(modifiers) {}
        
        const Expression& condition;
        const Arc::AbstractSyntaxTree& body;
        const Arc::AbstractSyntaxTree& elseBody;
        std::vector<Modifier> modifiers;
        
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::IfExpression; }
    };
}