/****************************************************************
 * @file GameSettingsRecordsFileSaver.h
 * @author Michal Dobes
 * @brief Game settings records file saver
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GAMESETTINGSRECORDSFILESAVER_H
#define GAMESETTINGSRECORDSFILESAVER_H

#include "Utilities/FileManagers/GameSettingsRecordsFileManager.h"

/**
 * @brief Game settings and records file saver
 *
 * Used for saving GameSettings and GameRecords to file.
 *
 */
class GameSettingsRecordsFileSaver : public GameSettingsRecordsFileManager {
public:

    /**
     * @brief Construct a new Game Settings Records File Saver object
     *
     * Tries to overwrite/create the file
     *
     * @throw FileLoaderException failed to utilize file
     *
     * @param filePath path to new game settings and records file
     */
    GameSettingsRecordsFileSaver(const std::string & filePath);

    /**
     * @brief Attempt to write settings and records into file
     *
     * @throw FileLoaderException Couln't save settings or records
     *
     * @param data pair of GameSettings and GameRecords to write into the file
     */
    void writeSettingsAndRecords(std::pair<GameSettings, GameRecords> data);

};

#endif /* GAMESETTINGSRECORDSFILESAVER_H */