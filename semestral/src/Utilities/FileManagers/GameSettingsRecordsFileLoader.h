/****************************************************************
 * @file GameSettingsRecordsFileLoader.h
 * @author Michal Dobes
 * @brief Game settings and records file loader
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMESETTINGSRECORDSFILELOADER_H
#define GAMESETTINGSRECORDSFILELOADER_H

#include "Utilities/FileManagers/GameSettingsRecordsFileManager.h"

/**
 * @brief Game settings and records file loader
 *
 * Used for loading GameSettings and GameRecords from file.
 *
 */
class GameSettingsRecordsFileLoader : public GameSettingsRecordsFileManager {
public:
    /**
     * @brief Construct a new Game Settings Records File Loader object
     *
     * Tries to open the file
     *
     * @throw FileLoaderException failed to utilize file
     *
     * @param filePath path to game settings and records file
     */
    GameSettingsRecordsFileLoader(const std::string & filePath);

    /**
     * @brief Attempts to load GameSettings and GameRecords from file
     *
     * @throw FileLoaderException Couln't load settings or records
     *
     * @return std::pair<GameSettings, GameRecords> loaded GameSettings and GameRecords
     */
    std::pair<GameSettings, GameRecords> loadSettingsAndRecords();

};

#endif /* GAMESETTINGSRECORDSFILELOADER_H */