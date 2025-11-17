#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct DebugWrite : IRInstruction
    {
        DebugWrite(
            const std::string& message, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : message(message), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const std::string& message;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::DebugWrite; }
    };
}