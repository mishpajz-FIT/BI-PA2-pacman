#ifndef FILELOADER_H
#define FILELOADER_H

#include <stdexcept>
#include <string>
#include <fstream>

class FileLoader {
protected:
    std::fstream file;

public:
    FileLoader(const std::string & filePath);
    virtual ~FileLoader();

};

struct FileLoaderException : public std::runtime_error {
    FileLoaderException(const std::string & message);
};

#endif /* BOARD_H */