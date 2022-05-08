#ifndef GAMEFILELOADER_H
#define GAMEFILELOADER_H

#include "FileLoader.h"
#include "Game.h"

class GameFileLoader : public FileLoader {
public:
    GameFileLoader(const std::string & filePath);

    Game loadGame();

};

#endif /* GAMEFILELOADER_H */