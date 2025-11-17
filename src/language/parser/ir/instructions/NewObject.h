#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct NewObject : IRInstruction
    {
        NewObject(
            const size_t structDirectiveId, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : structDirectiveId(structDirectiveId), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t structDirectiveId;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::NewObject; }
    };
}