#pragma once
#include "ASTExpression.h"
#include "../../ExpressionKind.h"
#include "../Modifier.h"
#include "../../../util/imports.h"
#include "../statements/VariableDeclaration.h"
#include "../statements/ASTStatement.h"

namespace ASTNode
{
    struct AnonymousVariableDeclaration : ASTExpression
    {
        AnonymousVariableDeclaration(
            const std::unique_ptr<VariableDeclaration>& variableDeclaration
        ) : signature(variableDeclaration->signature),constant(variableDeclaration->constant), datatype(variableDeclaration->datatype), forwardDeclared(variableDeclaration->forwardDeclared), value(variableDeclaration->value), modifiers(variableDeclaration->modifiers), decorators(variableDeclaration->decorators) {}
        
        const bool constant;
        const Expression& signature;
        const std::optional<Type>& datatype;
        const bool forwardDeclared;
        const std::optional<Expression>& value;
        const std::vector<Decorator>& decorators;
        std::vector<Modifier> modifiers;
        
        bool hasBody() const override { return false; }
        bool isLiteral() const override { return false; }
        Arc::ExpressionKind kind() const override { return  Arc::ExpressionKind::AnonymousVariableDeclaration; }
    };
}