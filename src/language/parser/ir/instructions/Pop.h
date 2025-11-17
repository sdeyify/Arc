#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Pop : IRInstruction
    {
        Pop(const std::vector<Metadata>& metadata, const size_t line, const size_t col) : IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Pop; }
    };
}