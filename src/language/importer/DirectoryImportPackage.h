#pragma once
#include "Importer.h"
#include "../util/Util.h"

namespace Arc
{
    struct DirectoryImportPackage : ImportPackage
    {
        DirectoryImportPackage(
            const std::string& path, const std::string& name, const std::vector<Package> children
        ) : path(path), name(name), children(children) {};
        
        const std::string& path;
        const std::string& name;
        const std::vector<Package>& children;

        ImportPackageKind kind() const override { return ImportPackageKind::DirectoryImport; }
    };
}