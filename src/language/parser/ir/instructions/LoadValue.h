#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct LoadValue : IRInstruction
    {
        LoadValue(
            const Arc::IRBitsizeKind& bitsize, const std::string& rawRepresentation, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : bitsize(bitsize), rawRepresentation(rawRepresentation), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const Arc::IRBitsizeKind& bitsize;
        const std::string& rawRepresentation;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::LoadValue; }
    };
}