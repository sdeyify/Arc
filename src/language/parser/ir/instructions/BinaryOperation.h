#include "IRInstruction.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRInstructionKind.h"
#include "../../IRBinaryOperationKind.h"
#include "../../IRBitsizeKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct BinaryOperation : IRInstruction
    {
        BinaryOperation(
            const Arc::IRBinaryOperationKind& op, const Arc::IRBitsizeKind& bitsize, const std::vector<Metadata>& metadata, const size_t line, const size_t col
        ) : op(op), bitsize(bitsize), IRInstruction(Arc::MetadataHandler(metadata), line, col) {}

        const Arc::IRBinaryOperationKind& op;
        const Arc::IRBitsizeKind& bitsize;

        Arc::IRInstructionKind kind() const override { return Arc::IRInstructionKind::BinaryOperation; }
    };
}