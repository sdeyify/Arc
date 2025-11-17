#pragma once
#include "ASTBodiedExpression.h"
#include "../../AbstractSyntaxTree.h"
#include "CatchExpression.h"
#include "../../../util/imports.h"
#include "../../ExpressionKind.h"
#include "../statements/ASTStatement.h"

namespace ASTNode
{
    struct TryCatchExpression : ASTBodiedExpression
    {
        TryCatchExpression (
            const Expression& tryBody, const std::optional<CatchExpression>& catchExpr, const std::optional <Arc::AbstractSyntaxTree>& catchBody
        ) : tryBody(tryBody), catchExpr(catchExpr), catchBody(catchBody) {}
        
        const Expression& tryBody;
        const std::optional<CatchExpression>& catchExpr;
        const std::optional <Arc::AbstractSyntaxTree>& catchBody;
        
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::TryCatchExpression; }
    };
}