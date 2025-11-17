#pragma once
#include "ASTStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../../util/imports.h"
#include "../../StatementKind.h"

namespace ASTNode
{
    struct ASTBodiedStatement : virtual ASTStatement
    {
        ASTBodiedStatement();

        virtual Arc::StatementKind kind() const = 0;
        bool hasBody() const override { return true; }
        
        const Arc::AbstractSyntaxTree& body;
    };

    using BodiedStatement = std::shared_ptr<ASTBodiedStatement>;
}

namespace Arc
{
    using BodiedStatement = std::shared_ptr<ASTNode::ASTBodiedStatement>;
}