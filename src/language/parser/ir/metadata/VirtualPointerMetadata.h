#pragma once
#include "IRMetadata.h"
#include "../../IRMetadataKind.h"

namespace IRNodes
{
    struct VirtualPointerMetadata : IRMetadata
    {
        VirtualPointerMetadata(const int64_t data) : data(data) {}

        const const int64_t data;

        Arc::IRMetadataKind kind() const override { return Arc::IRMetadataKind::VirtualPointer; }
    };
}