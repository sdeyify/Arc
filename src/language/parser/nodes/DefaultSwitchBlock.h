#pragma once
#include "../AbstractSyntaxTree.h"
#include "SwitchBlockKind.h"
#include "../../util/Util.h"
#include "statements/ASTStatement.h"
#include "ASTSwitchBlock.h"

namespace ASTNode
{
    struct DefaultSwitchBlock : ASTSwitchBlock
    {
        DefaultSwitchBlock(const Arc::AbstractSyntaxTree& body) : body(body) {}

        void operator=(DefaultSwitchBlock other) {
            this->body = other.body;
        }

        Arc::AbstractSyntaxTree body;
        SwitchBlockKind kind() const override { return SwitchBlockKind::DefaultCase; }
    };
}