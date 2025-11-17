#pragma once
#include "ASTType.h"
#include "../expressions/ASTExpression.h"
#include "../../TypeKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct MemberType : ASTType
    {
        MemberType(
            const Expression& owner, const Type& member
        ) : owner(owner), member(member) {}

        const Expression& owner;
        const Type& member;

        Arc::TypeKind kind() const override { return Arc::TypeKind::MemberType; }
    };
}