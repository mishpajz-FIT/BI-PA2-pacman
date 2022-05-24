#ifndef GAMESETTINGSRECORDSFILELOADER_H
#define GAMESETTINGSRECORDSFILELOADER_H

#include "Utilities/FileManagers/GameSettingsRecordsFileManager.h"

class GameSettingsRecordsFileLoader : public GameSettingsRecordsFileManager {
public:
    GameSettingsRecordsFileLoader(const std::string & filePath);

    std::pair<GameSettings, GameRecords> loadSettingsAndRecords();

};

#endif /* GAMESETTINGSRECORDSFILELOADER_H */