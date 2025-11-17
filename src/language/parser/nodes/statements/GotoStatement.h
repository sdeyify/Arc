#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct GotoStatement : ASTStatement
    {
        GotoStatement(const std::string& labelName) : labelName(labelName) {}
        
        const std::string& labelName;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::GotoStatement; }
    };
}