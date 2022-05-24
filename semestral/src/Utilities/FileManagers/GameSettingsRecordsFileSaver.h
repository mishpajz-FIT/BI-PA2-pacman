#ifndef GAMESETTINGSRECORDSFILESAVER_H
#define GAMESETTINGSRECORDSFILESAVER_H

#include "Utilities/FileManagers/GameSettingsRecordsFileManager.h"

class GameSettingsRecordsFileSaver : public GameSettingsRecordsFileManager {
public:
    GameSettingsRecordsFileSaver(const std::string & filePath);

    void writeSettingsAndRecords(std::pair<GameSettings, GameRecords> data);

};

#endif /* GAMESETTINGSRECORDSFILESAVER_H */