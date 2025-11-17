#pragma once
#include "ASTBodiedStatement.h"
#include "ASTModifiableStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"
#include "../DefaultSwitchBlock.h"
#include "../ValuedSwitchBlock.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct SwitchStatement : ASTModifiableStatement
    {
        SwitchStatement(
            const Expression& switchedValue, const std::vector<ValuedSwitchBlock>& valueCases, const std::optional<DefaultSwitchBlock>& defaultCase, std::vector<Modifier> modifiers
        ) : switchedValue(switchedValue), valueCases(valueCases), defaultCase(defaultCase), modifiers(modifiers) {}
        
        const Expression& switchedValue;
        const std::vector<ValuedSwitchBlock>& valueCases;
        const std::optional<DefaultSwitchBlock>& defaultCase;
        std::vector<Modifier> modifiers;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::SwitchStatement; }
    };
}