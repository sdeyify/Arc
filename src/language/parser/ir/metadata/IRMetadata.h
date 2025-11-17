#pragma once
#include "../../IRMetadataKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct IRMetadata
    {
        virtual Arc::IRMetadataKind kind() const = 0;

        template<std::derived_from<IRMetadata> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }
    };

    using Metadata = std::shared_ptr<IRMetadata>;
}

namespace Arc
{
    using Metadata = std::shared_ptr<IRNodes::IRMetadata>;
}