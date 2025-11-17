#pragma once
#include "../util/imports.h"

namespace Arc
{
    struct ASTBuildingContext {
        const bool parsingEnum = false;
        const std::optional<std::string>& parsingNewPath = std::nullopt;
    };
}