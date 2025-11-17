#include "ASTBodiedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct CatchHandleStatement : ASTBodiedStatement
    {
        CatchHandleStatement(const Arc::AbstractSyntaxTree& body) : body(body) {}

        const Arc::AbstractSyntaxTree& body;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::CatchHandleStatement; }
    };
}