#pragma once
#include "ASTType.h"
#include "../../TypeKind.h"

namespace ASTNode
{
    struct UnitType : ASTType
    {
        Arc::TypeKind kind() const override { return Arc::TypeKind::UnitType; }
    };
}