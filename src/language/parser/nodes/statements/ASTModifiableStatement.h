#pragma once
#include "ASTStatement.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../../StatementKind.h"

namespace ASTNode
{
    struct ASTModifiableStatement : virtual ASTStatement
    {
        ASTModifiableStatement();

        virtual Arc::StatementKind kind() const = 0;
        bool hasModifiers() const override { return true; }

        std::vector<Modifier> modifiers;
    };

    using ModifiableStatement = std::shared_ptr<ASTModifiableStatement>;
}

namespace Arc
{
    using ModifiableStatement = std::shared_ptr<ASTNode::ASTModifiableStatement>;
}