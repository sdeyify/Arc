#pragma once
#include "ASTExpression.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct ContractExpression : ASTExpression
    {
        ContractExpression(
            std::vector<Modifier> modifiers, const std::optional<Expression>& preCondition, const std::optional<Expression>& postCondition, const std::optional<Expression>& runCondition
        ) : modifiers(modifiers), preCondition(preCondition), postCondition(postCondition), runCondition(runCondition) {}

        std::vector<Modifier> modifiers;
        const std::optional<Expression>& preCondition;
        const std::optional<Expression>& postCondition;
        const std::optional<Expression>& runCondition;

        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return Arc::ExpressionKind::ContractExpression; }
    };
}