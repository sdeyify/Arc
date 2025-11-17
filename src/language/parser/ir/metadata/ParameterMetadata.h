#pragma once
#include "IRMetadata.h"
#include "../../IRMetadataKind.h"
#include "../../IRBitsizeKind.h"
#include "../../../util/imports.h"

namespace IRNodes
{
    struct ParameterMetadata : IRMetadata
    {
        ParameterMetadata(const std::vector<Arc::IRBitsizeKind>& data) : data(data) {}

        const std::vector<Arc::IRBitsizeKind>& data;

        Arc::IRMetadataKind kind() const override { return Arc::IRMetadataKind::Parameters; }
    };
}