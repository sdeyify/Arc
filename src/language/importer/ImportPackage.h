#pragma once
#include "ImportPackageKind.h"
#include "../util/Util.h"

namespace Arc
{
    struct ImportPackage
    {
        const std::string path;
        const std::string name;
        virtual ImportPackageKind kind() const = 0;
    };

    using Package = std::shared_ptr<ImportPackage>;
}