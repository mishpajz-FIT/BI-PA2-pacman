#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdexcept>
#include <string>
#include <fstream>

class FileManager {
protected:
    std::fstream file;

public:
    FileManager(const std::string & filePath, bool write = false);
    virtual ~FileManager();

};

struct FileLoaderException : public std::runtime_error {
    FileLoaderException(const std::string & message);
};

#endif /* FILEMANAGER_H */