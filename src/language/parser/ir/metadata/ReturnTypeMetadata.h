#pragma once
#include "IRMetadata.h"
#include "../../IRMetadataKind.h"
#include "../../IRBitsizeKind.h"

namespace IRNodes
{
    struct ReturnTypeMetadata : IRMetadata
    {
        ReturnTypeMetadata(const Arc::IRBitsizeKind& data) : data(data) {}

        const Arc::IRBitsizeKind& data;

        Arc::IRMetadataKind kind() const override { return Arc::IRMetadataKind::ReturnType; }
    };
}