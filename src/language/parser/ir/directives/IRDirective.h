#pragma once
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRDirectiveKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct IRDirective : std::enable_shared_from_this<IRDirective>
    {
        IRDirective(
            const size_t id, const Arc::MetadataHandler& metadata, const size_t line, const size_t col
        ) : id(id), metadata(metadata), line(line), col(col) {}

        const Arc::MetadataHandler& metadata;
        const size_t id;
        const size_t line;
        const size_t col;

        template<std::derived_from<IRDirective> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }

        virtual Arc::IRDirectiveKind kind() const = 0;
    };

    using Directive = std::shared_ptr<IRDirective>;
}

namespace Arc
{
    using Directive = std::shared_ptr<IRNodes::IRDirective>;
}