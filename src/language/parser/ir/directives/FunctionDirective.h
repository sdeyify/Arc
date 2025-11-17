#pragma once
#include "IRDirective.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRDirectiveKind.h"

namespace IRNodes
{
    struct FunctionDirective : IRDirective
    {
        FunctionDirective(
            const size_t id, const size_t implementationAddress, const std::vector<Metadata> metadata, const size_t line, const size_t col
        ) : implementationAddress(implementationAddress), IRDirective(id, Arc::MetadataHandler(metadata), line, col) {}

        const size_t implementationAddress;

        Arc::IRDirectiveKind kind() const override { return Arc::IRDirectiveKind::FunctionDirective; }
    };
}