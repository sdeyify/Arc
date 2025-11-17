#pragma once
#include "../util/Util.h"
#include "ImportPackage.h"
#include "FileImportPackage.h"
#include "DirectoryImportPackage.h"

namespace Arc
{
    class ArcImporter
    {
    public:
        ArcImporter(const std::string& path) : path(path) {}
    private:
        std::map<std::string, std::string> aliases;
        std::vector<std::string> usedAliases;
        const std::string& path;
    public:
        Package resolveImport(const std::string& path);
        void addAlias(const std::string& alias, const std::string& value);
        void clearAliases();
        bool usedAlias(const std::string& alias);
        bool isAlias(const std::string& alias);
    private:
        std::string resolveFilePath(const std::string& originalPath);
        std::string readFile(const std::string& path);
        DirectoryImportPackage readDirectory(const std::string& path);
        bool fileExists(const std::string& path);
        bool directoryExists(const std::string& path);
    };
}