#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct BranchIf : IRInstruction
    {
        BranchIf(
            const size_t labelId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : labelId(labelId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t labelId;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::BranchIf; }
    };
}