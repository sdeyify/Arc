#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct StoreLocal : IRInstruction
    {
        StoreLocal(
            const size_t localId, const Arc::IRBitsizeKind& bitsize, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : localId(localId), bitsize(bitsize), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t localId;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::StoreLocal; }
    };
}