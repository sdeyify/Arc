#pragma once
#include "../../util/imports.h"
namespace ASTNode
{
    struct Modifier
    {
        Modifier(const std::string& identifer) : identifier(identifier) {}
        
        const std::string& identifier;
    };
}