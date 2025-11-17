#pragma once
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct IRInstruction : std::enable_shared_from_this<IRInstruction>
    {
        IRInstruction(
            const Arc::MetadataHandler& metadata, const size_t line, const size_t col
        ) : metadata(metadata), line(line), col(col) {}

        const Arc::MetadataHandler& metadata;
        const size_t line;
        const size_t col;

        template<std::derived_from<IRInstruction> T>
        std::shared_ptr<T> cast() const { return std::dynamic_pointer_cast<T>(shared_from_this()); }

        virtual Arc::IRInstructionKind kind() const = 0;
    };

    using Instruction = std::shared_ptr<IRInstruction>;
}

namespace Arc
{
    using Instruction = std::shared_ptr<IRNodes::IRInstruction>;
}