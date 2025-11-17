#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct LoadDirectivePointer : IRInstruction
    {
        LoadDirectivePointer(
            const size_t directiveId, const Arc::IRBitsizeKind& bitsize, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : directiveId(directiveId), bitsize(bitsize), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t directiveId;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::LoadDirectivePointer; }
    };
}