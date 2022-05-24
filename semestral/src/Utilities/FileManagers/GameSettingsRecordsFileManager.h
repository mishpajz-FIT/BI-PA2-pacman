#ifndef GAMESETTINGSRECORDSFILEMANAGER_H
#define GAMESETTINGSRECORDSFILEMANAGER_H

#include <array>
#include <string>
#include <tuple>

#include "Utilities/FileManagers/FileManager.h"
#include "Utilities/Contexts/GameSettings.h"
#include "Utilities/Contexts/GameRecords.h"

class GameSettingsRecordsFileManager : public FileManager {
protected:
    static const std::array<std::string, 8> keys;

public:

    GameSettingsRecordsFileManager(const std::string & filepath, bool write = false);

    virtual ~GameSettingsRecordsFileManager();
};

#endif /* GAMESETTINGSRECORDSFILEMANAGER_H */