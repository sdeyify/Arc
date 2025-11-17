#pragma once
#include "Importer.h"
#include "../util/Util.h"
#include "../messages/Messages.h"

void Arc::ArcImporter::addAlias(const std::string& alias, const std::string& value) {
    this->aliases[alias] = value;
}

void Arc::ArcImporter::clearAliases() {
    this->aliases.clear();
}

bool Arc::ArcImporter::isAlias(const std::string& alias) {
    if (this->aliases.contains(alias)) {
        this->usedAliases.push_back(alias);
        return true;
    }

    return false;
}

bool Arc::ArcImporter::usedAlias(const std::string& alias) {
    return Util::includes<std::string>(this->usedAliases, alias);
}

bool Arc::ArcImporter::fileExists(const std::string& path) {
    return std::filesystem::exists(path) && !std::filesystem::is_directory(path);
}

bool Arc::ArcImporter::directoryExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

Arc::Package Arc::ArcImporter::resolveImport(const std::string& path) {
    const std::string& resolvedPath = this->resolveFilePath(path);

    if (this->fileExists(resolvedPath)) {
        return std::make_shared<Arc::FileImportPackage> (
            resolvedPath,
            std::filesystem::directory_entry(resolvedPath).path().filename().string(),
            this->readFile(resolvedPath)
        );
    } else if (this->directoryExists(resolvedPath)) {
        return std::make_shared<DirectoryImportPackage>(this->readDirectory(resolvedPath));
    }

    Arc::ArcLogger::fatal_errln(std::format("Could not resolve import: {0}", resolvedPath));
    std::unreachable();
}

Arc::DirectoryImportPackage Arc::ArcImporter::readDirectory(const std::string& path) {
    const std::filesystem::directory_iterator directoryIterator = std::filesystem::directory_iterator(path);
    std::vector<Package> children;

    for (const std::filesystem::directory_entry& entry : directoryIterator) {
        children.push_back(this->resolveImport(entry.path().string()));
    }

    return DirectoryImportPackage (
        path,
        directoryIterator->path().filename().string(),
        children
    );
}

std::string Arc::ArcImporter::resolveFilePath(const std::string& originalPath) {
    std::vector<std::string> splitPath = Util::split(this->path + "/" + originalPath, {'/', '\\'});
    std::vector<std::string> resolvedPath;

    while (splitPath.size() > 0) {
        const std::string entry = splitPath.front();
        if (entry == ".") {
            resolvedPath.pop_back();
        } else if (entry == "..") {
            resolvedPath.pop_back();
            resolvedPath.pop_back();
        } else if (this->isAlias(entry)) {
            resolvedPath.push_back(this->aliases[entry]);
        } else {
            resolvedPath.push_back(entry);
        }
        splitPath.erase(splitPath.begin());
    }

    return Util::join<std::string, char>(resolvedPath, '/');
}

std::string Arc::ArcImporter::readFile(const std::string& path) {
    std::ifstream inputFile(path);

    if (!inputFile.is_open()) {
        Arc::ArcLogger::fatal_errln(std::format("Could not open file: {0}", path));
    }

    std::string line;
    std::string contents;
    while (std::getline(inputFile, line)) {
        contents += line;
    }

    inputFile.close();   
    return contents;
}