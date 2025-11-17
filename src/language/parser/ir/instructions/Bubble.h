#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Bubble : IRInstruction
    {
        Bubble(
            const size_t stackAddress, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : stackAddress(stackAddress), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const size_t stackAddress;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Bubble; }
    };
}