#include "FileLoader.h"

#include <iostream>
#include <filesystem>

FileLoader::FileLoader(const std::string & filePath) : file(filePath, std::ios::in) {
    if (!file.is_open() || !file.good()) {
        throw FileLoaderException("fileloader: error utilizing file");
    }
}

FileLoader::~FileLoader() {
    file.close();
}

FileLoaderException::FileLoaderException(const std::string & message) : runtime_error(message) { }