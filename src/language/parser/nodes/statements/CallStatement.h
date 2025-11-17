#pragma once
#include "../Type.h"
#include "ASTStatement.h"
#include "../../StatementKind.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct CallStatement : ASTStatement
    {
        CallStatement(
            const Expression& signature, const std::vector<Type>& generics, const std::vector<Expression>& arguments
        ) : signature(signature), generics(generics), arguments(arguments) {}

        const Expression& signature;
        const std::vector<Type>& generics;
        const std::vector<Expression>& arguments;

        bool hasBody() const override { return false; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return false; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::CallStatement; }
    };
}