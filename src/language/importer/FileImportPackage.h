#pragma once
#include "Importer.h"
#include "../util/Util.h"

namespace Arc
{
    struct FileImportPackage : ImportPackage
    {
        FileImportPackage(
            const std::string& path, const std::string& name, const std::string& contents
        ) : path(path), contents(contents), name(name) {};
        
        const std::string& path;
        const std::string& name;
        const std::string& contents;

        ImportPackageKind kind() const override { return ImportPackageKind::FileImport; }
    };
}