#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Exit : IRInstruction
    {
        Exit(
            const int exitCode, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : exitCode(exitCode), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const int exitCode;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Exit; }
    };
}