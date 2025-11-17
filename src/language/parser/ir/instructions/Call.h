#pragma once
#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Call : IRInstruction
    {
        Call(
            const size_t functionDirectiveId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : functionDirectiveId(functionDirectiveId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t functionDirectiveId;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Call; }
    };
}