#include "Utilities/FileManagers/BoardFileLoader.h"

bool BoardFileLoader::checkForSpecialCharacter(char c, size_t x, size_t y) {
    switch (c) { // If chars are 'P' or 'E', set spawn positions (or throw is spawn has already been set)
        case 'P':
            if (playerSpawn.x == -1 && playerSpawn.y == -1) {
                playerSpawn = Position(x, y);
                return true;
            } else {
                throw FileLoaderException("BoardFileLoader: checkForSpecialCharacter - duplicit player spawn");
            }
            return true;
        case 'E':
            if (enemySpawn.x == -1 && enemySpawn.y == -1) {
                enemySpawn = Position(x, y);
            } else {
                throw FileLoaderException("BoardFileLoader: checkForSpecialCharacter - duplicit enemy spawn");
            }
            return true;
        default:
            break;
    }
    return false;
}

BoardFileLoader::TileMatrix BoardFileLoader::createTilesOutOfData(std::list<std::string> & lines) {
    BoardFileLoader::TileMatrix generatedTiles(lines.front().size(), lines.size());

    size_t y = 0;
    size_t x = 0;
    for (auto & l : lines) {
        for (auto & c : l) {
            // For each character in each line, check if is special then replace 
            // it by default, else convert it to correct type
            if (checkForSpecialCharacter(c, x, y)) {
                generatedTiles.at(x++, y) = Board::Tile::defaultType();
            } else {
                generatedTiles.at(x++, y) = dataCharToType(c);
            }
        }
        x = 0;
        y++;
    }

    if ((enemySpawn.x == -1 || enemySpawn.y == -1)
        || (playerSpawn.x == -1 || playerSpawn.y == -1)) { //Check if spawns have been set
        throw FileLoaderException("BoardFileLoader: createTilesOutOfData - missing spawn point");
    }

    return generatedTiles;
}

Board::Tile::Type BoardFileLoader::dataCharToType(char c) {
    switch (c) { // Map chars to types
        case '#':
            return Board::Tile::Type::wall;
            break;
        case '.':
            return Board::Tile::Type::coin;
        case 'o':
            return Board::Tile::Type::frighten;
        case ' ':
            break;
        default:
            throw FileLoaderException("BoardFileLoader: dataCharToType - unknown char in file");
            break;
    }

    return Board::Tile::Type::space;
}

BoardFileLoader::BoardFileLoader(const std::string & filePath)
    :
    FileManager(filePath),
    playerSpawn(-1, -1),
    enemySpawn(-1, -1) { }

Board BoardFileLoader::loadBoard() {
    std::list<std::string> lines;

    std::string buffer;
    size_t lineLength = 0;

    while (std::getline(file, buffer)) { // Read lines from file

        if (lineLength == 0) {
             // If line is first with content set line length, else check if 
             // loaded line length is correct
            lineLength = buffer.length();
        } else if (lineLength != buffer.length()) {
            throw FileLoaderException("BoardFileLoader: loadBoard - wrong format of grid in file");
        }

        lines.emplace_back(buffer);
    }

    if (lines.size() == 0) {
        throw FileLoaderException("BoardFileLoader: loadBoard - empty grid");
    }

    return Board(createTilesOutOfData(lines), enemySpawn, playerSpawn);
}