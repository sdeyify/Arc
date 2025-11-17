#pragma once
#include "../../util/Util.h"
#include "InterpolationKind.h"

namespace ASTNode
{
    struct Interpolation
    {
        virtual InterpolationKind kind() const = 0;
    };

    using StringInterpolation = std::shared_ptr<Interpolation>;
}