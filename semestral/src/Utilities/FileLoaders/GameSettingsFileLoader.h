#ifndef GAMESETTINGSFILELOADER_H
#define GAMESETTINGSFILELOADER_H

#include "FileLoader.h"
#include "GameSettings.h"

class GameSettingsFileLoader : public FileLoader {
public:
    GameSettingsFileLoader(const std::string & filePath);

    GameSettings loadSettings();

};

#endif /* GAMESETTINGSFILELOADER_H */