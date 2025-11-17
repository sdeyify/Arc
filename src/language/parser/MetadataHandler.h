#pragma once
#include "IRMetadataKind.h"
#include "ir/IRNodes.h"
#include "../util/imports.h"

namespace Arc
{
    class MetadataHandler
    {
    public:
        MetadataHandler(const std::vector<IRNodes::Metadata>& metadata) : metadata(this->parseMetadata(metadata)) {};
    private:
        const std::map<IRMetadataKind, IRNodes::Metadata>& metadata;
    public:
        std::optional<IRNodes::Metadata> get(const Arc::IRMetadataKind& kind) const;
        IRNodes::Metadata expect(const Arc::IRMetadataKind& kind, const std::string& errorMessage) const;

        template<std::derived_from<IRNodes::IRMetadata> T>
        std::optional<std::shared_ptr<T>> getCast(const Arc::IRMetadataKind& kind) const {
            return (this->metadata.contains(kind)) ? this->get(kind).value()->cast<T>() : std::nullopt;
        }

        template<std::derived_from<IRNodes::IRMetadata> T>
        std::shared_ptr<T> expectCast(const Arc::IRMetadataKind& kind, const std::string& errorMessage) const {
            return this->expect(kind, "")->cast<T>();
        }
    private:
        std::map<IRMetadataKind, IRNodes::Metadata> parseMetadata(const std::vector<IRNodes::Metadata>& metadata) const;
    };
}