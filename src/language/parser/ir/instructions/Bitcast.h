#pragma once
#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Bitcast : IRInstruction
    {
        Bitcast(
            const Arc::IRBitsizeKind& startType, const Arc::IRBitsizeKind& endType, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : startType(startType), endType(endType), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const Arc::IRBitsizeKind& startType;
        const Arc::IRBitsizeKind& endType;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Bitcast; }
    };
}