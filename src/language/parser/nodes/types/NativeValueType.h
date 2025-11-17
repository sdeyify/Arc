#pragma once
#include "ASTType.h"
#include "../../IRBitsizeKind.h"
#include "../../TypeKind.h"
#include "../../../util/imports.h"

namespace ASTNode
{
    struct NativeValueType : ASTType
    {
        NativeValueType(const Arc::IRBitsizeKind& bitsize) : bitsize(bitsize) {}

        const Arc::IRBitsizeKind& bitsize;

        Arc::TypeKind kind() const override { return Arc::TypeKind::NativeValueType; }
    };
} 