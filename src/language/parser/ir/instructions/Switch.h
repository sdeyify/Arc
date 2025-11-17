#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct Switch : IRInstruction
    {
        Switch(
            const std::vector<size_t>& switchLabelIds, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : switchLabelIds(switchLabelIds), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const std::vector<size_t>& switchLabelIds;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::Switch; }
    };
}