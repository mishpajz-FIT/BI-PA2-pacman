/****************************************************************
 * @file GameSettingsRecordsFileManager.h
 * @author Michal Dobes
 * @brief Game settings records file manager
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMESETTINGSRECORDSFILEMANAGER_H
#define GAMESETTINGSRECORDSFILEMANAGER_H

#include <array>
#include <string>
#include <tuple>

#include "Utilities/FileManagers/FileManager.h"
#include "Utilities/Contexts/GameSettings.h"
#include "Utilities/Contexts/GameRecords.h"

/**
 * @brief File manager for game settings and record file
 *
 */
class GameSettingsRecordsFileManager : public FileManager {
protected:
    static const std::array<std::string, 8> keys; //< Keys in settings file

public:

    /**
     * @brief Construct a new File Settings Records File Manager object
     *
     * Tries to open the file
     *
     * @throw FileLoaderException failed to utilize file
     *
     * @param filePath path to file
     * @param write open for write, else opens for read
     */
    GameSettingsRecordsFileManager(const std::string & filepath, bool write = false);

    /**
     * @brief Destroy the Game Settings Records File Manager object
     *
     */
    virtual ~GameSettingsRecordsFileManager();
};

#endif /* GAMESETTINGSRECORDSFILEMANAGER_H */