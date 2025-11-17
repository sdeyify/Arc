#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"

namespace ASTNode
{
    struct PipelineExpression : ASTExpression
    {
        PipelineExpression(
            const Expression& streamed, const std::vector<Expression>& pipelines
        ) : streamed(streamed), pipelines(pipelines) {}
        
        const Expression& streamed;
        const std::vector<Expression>& pipelines;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::PipelineExpression; }
    };
}