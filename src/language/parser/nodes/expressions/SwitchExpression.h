#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../DefaultSwitchBlock.h"
#include "../ValuedSwitchBlock.h"
#include "../Modifier.h"

namespace ASTNode
{
    struct SwitchExpression : ASTExpression
    {
        SwitchExpression(
            const Expression& switchedValue, const std::vector<ValuedSwitchBlock>& valueCases, const std::optional<DefaultSwitchBlock>& defaultCase, std::vector<Modifier> modifiers
        ) : switchedValue(switchedValue), valueCases(valueCases), defaultCase(defaultCase), modifiers(modifiers) {}
        
        const Expression& switchedValue;
        const std::vector<ValuedSwitchBlock>& valueCases;
        const std::optional<DefaultSwitchBlock>& defaultCase;
        std::vector<Modifier> modifiers;
        
        bool isLiteral() const override { return false; }
        bool hasBody() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::SwitchExpression; }
    };
}