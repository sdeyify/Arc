#pragma once
#include "ASTBodiedStatement.h"
#include "ASTNamedStatement.h"
#include "../../AbstractSyntaxTree.h"
#include "../../StatementKind.h"
#include "../Type.h"
#include "../Parameter.h"
#include "../Generic.h"
#include "../../../util/imports.h"
#include "../expressions/ASTExpression.h"

namespace ASTNode
{
    struct EnumMember : ASTBodiedStatement, ASTNamedStatement
    {
        EnumMember(
            const Expression& signature, const std::optional<std::vector<Generic>>& generics, const std::optional<std::vector<Parameter>>& parameters,
            const std::optional <Arc::AbstractSyntaxTree>& body, const std::optional<Expression> defaultValue
        ) : signature(signature), generics(generics), parameters(parameters), body(body), defaultValue(defaultValue) {}

        const Expression& signature;
        const std::optional<std::vector<Generic>>& generics;
        const std::optional<std::vector<Parameter>>& parameters;
        const std::optional <Arc::AbstractSyntaxTree>& body;
        const std::optional<Expression>& defaultValue;

        bool hasBody() const override { return true; }
        bool hasModifiers() const override { return false; }
        bool hasName() const override { return true; }
        Arc::StatementKind kind() const override { return Arc::StatementKind::EnumMember; }
    };
}