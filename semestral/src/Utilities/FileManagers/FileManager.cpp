#include <iostream>
#include <filesystem>

#include "Utilities/FileManagers/FileManager.h"

FileManager::FileManager(const std::string & filePath, bool write)
    : file(filePath, (write) ? (std::ios::out) : (std::ios::in)) {
    if (!file.is_open() || !file.good()) {
        throw FileLoaderException("FileManager: error utilizing file");
    }
}

FileManager::~FileManager() {
    file.close();
}

FileLoaderException::FileLoaderException(const std::string & message) : runtime_error(message) { }