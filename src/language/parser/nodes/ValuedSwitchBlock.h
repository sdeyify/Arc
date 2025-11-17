#pragma once
#include "../AbstractSyntaxTree.h"
#include "SwitchBlockKind.h"
#include "../../util/Util.h"
#include "statements/ASTStatement.h"
#include "expressions/ASTExpression.h"
#include "ASTSwitchBlock.h"

namespace ASTNode
{
    struct ValuedSwitchBlock : ASTSwitchBlock
    {
        ValuedSwitchBlock(
            const Arc::AbstractSyntaxTree& body, const std::optional<Expression>& value
        ) : body(body), value(value) {}

        void operator=(ValuedSwitchBlock other) {
            this->body = other.body;
        }

        Arc::AbstractSyntaxTree body;
        const std::optional<Expression>& value;

        SwitchBlockKind kind() const override { return SwitchBlockKind::ValuedCase; };
    };
}