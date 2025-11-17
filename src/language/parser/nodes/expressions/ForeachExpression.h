#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "IteratorExpression.h"
#include "../Modifier.h"
#include "../statements/ASTStatement.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct ForeachExpression : ASTBodiedExpression
    {
        ForeachExpression(
            const Arc::AbstractSyntaxTree& body, const IteratorExpression& iterator, std::vector<Modifier> modifiers
        ) : body(body), iterator(iterator), modifiers(modifiers) {}
        
        const Arc::AbstractSyntaxTree& body;
        const IteratorExpression& iterator;
        std::vector<Modifier> modifiers;
        
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::ForeachExpression; }
    };
}