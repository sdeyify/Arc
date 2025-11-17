#pragma once
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct LabelStatement : ASTStatement
    {
        LabelStatement(const std::string& labelName) : labelName(labelName) {}

        const std::string& labelName;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return true; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::LabelStatement; }
    };
}