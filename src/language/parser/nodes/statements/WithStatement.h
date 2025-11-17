#include "ASTBodiedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../Modifier.h"
#include "../expressions/ASTExpression.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct WithStatement : ASTBodiedStatement
    {
        WithStatement(
            const Expression& expr, const std::optional <Arc::AbstractSyntaxTree>& body
        ) : expr(expr), body(body) {}
        
        const Expression& expr;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        
        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::WithStatement; }
    };
}