#pragma once
#include "IRDirective.h"
#include "../metadata/IRMetadata.h"
#include "../../MetadataHandler.h"
#include "../../IRDirectiveKind.h"
#include "../../IRBitsizeKind.h"
#include "../../../util/imports.h"
#include "../../../simulator/StackValueKind.h"

namespace IRNodes
{
    struct StructDirective : IRDirective
    {
        StructDirective(
            const size_t id, const std::map<int64_t, Arc::IRBitsizeKind>& fields, const std::map<int64_t, Arc::StackValueKind>& fieldValueKinds, const std::vector<Metadata> metadata, const size_t line, const size_t col
        ) : fields(fields), fieldValueKinds(fieldValueKinds), IRDirective(id, Arc::MetadataHandler(metadata), line, col) {}

        const std::map<int64_t, Arc::IRBitsizeKind>& fields;
        const std::map<int64_t, Arc::StackValueKind>& fieldValueKinds;

        Arc::IRDirectiveKind kind() const override { return Arc::IRDirectiveKind::StructDirective; }
    };
}