#pragma once
#include "../util/imports.h"
#include "nodes/statements/ASTStatement.h"
#include "nodes/ASTNode.h"
#include "AbstractSyntaxTree.h"
#include "ir/IRNodes.h"
#include "IRProgram.h"

namespace Arc
{
    class IRGenerator
    {
    public:
        IRProgram generateIR(const AbstractSyntaxTree& ast);
    };
}