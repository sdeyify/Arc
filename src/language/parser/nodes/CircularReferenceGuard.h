#pragma once
#include "Statement.h"
#include "../../util/imports.h"
#include "../AbstractSyntaxTree.h"

namespace ASTNode
{
    struct CircularReferenceGuard
    {
        CircularReferenceGuard(
            const Arc::AbstractSyntaxTree& symbolReplacements, const std::string& guardedPath, const std::string& guardedFor
        ) : symbolReplacements(symbolReplacements), guardedPath(guardedPath), guardedFor(guardedFor) {}

        const Arc::AbstractSyntaxTree& symbolReplacements;
        const std::string& guardedPath;
        const std::string& guardedFor;
    };
}