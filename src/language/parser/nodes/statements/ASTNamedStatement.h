#pragma once
#include "ASTStatement.h"
#include "../../../util/imports.h"
#include "../../StatementKind.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct ASTNamedStatement : virtual ASTStatement
    {
        ASTNamedStatement();

        virtual Arc::StatementKind kind() const = 0;
        bool hasName() const override { return true; }

        const Expression& signature;
    };

    using NamedStatement = std::shared_ptr<ASTNamedStatement>;
}

namespace Arc
{
    using NamedStatement = std::shared_ptr<ASTNode::ASTNamedStatement>;
}