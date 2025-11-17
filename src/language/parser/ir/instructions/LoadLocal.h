#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct LoadLocal : IRInstruction
    {
        LoadLocal(
            const Arc::IRBitsizeKind& bitsize, const size_t localId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : bitsize(bitsize), localId(localId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t localId;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::LoadLocal; }
    };
}