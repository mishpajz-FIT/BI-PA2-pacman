#include "FileLoader.h"

FileLoader::FileLoader(const std::string & filePath) : file(filePath) {
    if (!file.good()) {
        throw FileLoaderException("fileloader: error utilizing file");
    }
}

FileLoader::~FileLoader() {
    file.close();
}

FileLoaderException::FileLoaderException(const std::string & message) : runtime_error(message) { }