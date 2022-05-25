/****************************************************************
 * @file FileManager.h
 * @author Michal Dobes
 * @brief File manager
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdexcept>
#include <string>
#include <fstream>

/**
 * @brief File manager
 *
 * Used for managing file loading/storing.
 *
 */
class FileManager {
protected:
    std::fstream file; //< file to utilize

public:
    /**
     * @brief Construct a new File Manager object
     *
     * Tries to open the file
     *
     * @throw FileLoaderException failed to utilize file
     *
     * @param filePath path to file
     * @param write open for write, else opens for read
     */
    FileManager(const std::string & filePath, bool write = false);

    /**
     * @brief Destroy the File Manager object
     *
     * Closes the file if possible
     *
     */
    virtual ~FileManager();

};

/**
 * @brief FileLoader exception
 *
 */
struct FileLoaderException : public std::runtime_error {
    FileLoaderException(const std::string & message);
};

#endif /* FILEMANAGER_H */