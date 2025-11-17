#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRBitsizeKind.h"
#include "../../IRInstructionKind.h"
#include "../../IRUnaryOperationKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct UnaryOperation : IRInstruction
    {
        UnaryOperation(
            const Arc::IRUnaryOperationKind& op, const Arc::IRBitsizeKind& bitsize, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : op(op), bitsize(bitsize), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const Arc::IRUnaryOperationKind& op;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::UnaryOperation; }
    };
}