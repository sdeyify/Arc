#include "Parser.h"
#include "../util/imports.h"

std::map<Arc::IRMetadataKind, Arc::IRNodes::Metadata> Arc::MetadataHandler::parseMetadata(const std::vector<Arc::IRNodes::Metadata>& metadata) const {
    std::map<Arc::IRMetadataKind, Arc::IRNodes::Metadata> mapped;

    for (const Arc::IRNodes::Metadata& metadataEntry : metadata) {
        mapped[metadataEntry->kind()] = metadataEntry;
    }

    return mapped;
}

std::optional<Arc::IRNodes::Metadata> Arc::MetadataHandler::get(const Arc::IRMetadataKind& kind) const {
    return (this->metadata.contains(kind)) ? std::make_optional(this->metadata.at(kind)) : std::nullopt;
}

Arc::IRNodes::Metadata Arc::MetadataHandler::expect(const Arc::IRMetadataKind& kind, const std::string& errorMessage) const {
    if (!this->metadata.contains(kind)) {
        // throw err
        std::unreachable();
    }

    return *this->get(kind);
}