#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct LoadField : IRInstruction
    {
        LoadField(
            const size_t structId, const Arc::IRBitsizeKind& bitsize, const uint32_t fieldId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : bitsize(bitsize), fieldId(fieldId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t structId;
        const uint32_t fieldId;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::LoadField; }
    };
}