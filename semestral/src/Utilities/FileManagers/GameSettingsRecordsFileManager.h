#ifndef GAMESETTINGSRECORDSFILEMANAGER_H
#define GAMESETTINGSRECORDSFILEMANAGER_H

#include "FileManager.h"
#include <array>
#include <string>
#include "GameSettings.h"
#include "GameRecords.h"
#include <tuple>

class GameSettingsRecordsFileManager : public FileManager {
protected:
    static const std::array<std::string, 8> keys;

public:

    GameSettingsRecordsFileManager(const std::string & filepath, bool write = false);

    virtual ~GameSettingsRecordsFileManager();
};

#endif /* GAMESETTINGSRECORDSFILEMANAGER_H */