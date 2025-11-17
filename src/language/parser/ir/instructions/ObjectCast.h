#pragma once
#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct ObjectCast : IRInstruction
    {
        ObjectCast(
            const size_t startStructId, const size_t endStructId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : startStructId(startStructId), endStructId(endStructId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t startStructId;
        const size_t endStructId;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::ObjectCast; }
    };
}