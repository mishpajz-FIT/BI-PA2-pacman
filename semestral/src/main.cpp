#include "Game.h"
#include "GameFileLoader.h"

#include <filesystem>
#include <iostream>

int main(void) {

    GameFileLoader gLoader("./examples/Settings/default.spac");

    Game game = gLoader.loadGame();
    game.loadMap("./examples/Maps/default.mpac");

    game.begin();
    while (true) {
        game.update();
    }
}