#pragma once
#include "../AbstractSyntaxTree.h"
#include "SwitchBlockKind.h"
#include "../../util/Util.h"
#include "statements/ASTStatement.h"

namespace ASTNode
{
    struct ASTSwitchBlock
    {
        Arc::AbstractSyntaxTree body;
        std::optional<Expression> value;
        virtual SwitchBlockKind kind() const = 0;
    };

    using SwitchBlock = std::shared_ptr<ASTSwitchBlock>;
}