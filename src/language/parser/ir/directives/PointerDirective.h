#pragma once
#include "IRDirective.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRDirectiveKind.h"
#include "../../IRBitsizeKind.h"

namespace IRNodes
{
    struct PointerDirective : IRDirective
    {
        PointerDirective(
            const size_t id, const Arc::IRBitsizeKind& bitsize, const std::string& rawRepresentation, const std::vector<Metadata> metadata, const size_t line, const size_t col
        ) : bitsize(bitsize), rawRepresentation(rawRepresentation), IRDirective(id, Arc::MetadataHandler(metadata), line, col) {}

        const Arc::IRBitsizeKind& bitsize;
        const std::string& rawRepresentation;

        Arc::IRDirectiveKind kind() const override { return Arc::IRDirectiveKind::PointerDirective; }
    };
}