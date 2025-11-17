#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../IRBitsizeKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct StorePointer : IRInstruction
    {
        StorePointer(
            const int64_t offset, const Arc::IRBitsizeKind& bitsize, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : offset(offset), bitsize(bitsize), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const int64_t offset;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::StorePointer; }
    };
}