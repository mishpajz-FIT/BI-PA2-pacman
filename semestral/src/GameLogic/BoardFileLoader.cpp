#include "BoardFileLoader.h"

bool BoardFileLoader::buildCheckForSpecialCharacter(char c, size_t x, size_t y, Board & toBoard) {
    switch (c) { //If chars are 'P' or 'E', set spawn positions (or throw is spawn has already been set)
        case 'P':
            if (toBoard.playerSpawn.x == -1 && toBoard.playerSpawn.y == -1) {
                toBoard.playerSpawn = Position(x, y);
                return true;
            } else {
                throw FileLoaderException("boardfileloader: duplicit player spawn");
            }
            return true;
        case 'E':
            if (toBoard.enemySpawn.x == -1 && toBoard.enemySpawn.y == -1) {
                toBoard.enemySpawn = Position(x, y);
            } else {
                throw FileLoaderException("boardfileloader: duplicit enemy spawn");
            }
            return true;
        default:
            break;
    }
    return false;
}

void BoardFileLoader::createBoardOutOfData(std::list<std::string> & lines, Board & toBoard) {
    Matrix<Board::Tile::Type> generatedTiles(lines.front().size(), lines.size());

    size_t y = 0;
    size_t x = 0;
    for (auto & l : lines) {
        for (auto & c : l) { //For each character in each line, check if is special then replace it by default, else convert it to correct type
            if (buildCheckForSpecialCharacter(c, x, y, toBoard)) {
                generatedTiles.at(x++, y) = Board::Tile::defaultType();
            } else {
                generatedTiles.at(x++, y) = dataCharToType(c);
            }
        }
        x = 0;
        y++;
    }

    if ((toBoard.enemySpawn.x == -1 && toBoard.enemySpawn.y == -1)
        || (toBoard.playerSpawn.x == -1 && toBoard.playerSpawn.y == -1)) { //Check if spawns have been set
        throw FileLoaderException("boardfileloader: missing spawn point");
    }

    toBoard.tiles = generatedTiles;
}

Board BoardFileLoader::loadBoard() {
    std::list<std::string> lines;

    std::string buffer;
    size_t lineLength = 0;

    while (std::getline(file, buffer)) { //Read lines from file

        if (lineLength == 0) { //If line is first with content set line length, else check if loaded line length is correct
            lineLength = buffer.length();
        } else if (lineLength != buffer.length()) {
            throw FileLoaderException("boardfileloader: wrong format of grid in file");
        }

        lines.emplace_back(buffer);
    }

    if (lines.size() == 0) {
        throw FileLoaderException("boardfileloader: empty grid");
    }

    Board board;
    createBoardOutOfData(lines, board);

    return board;
}

Board::Tile::Type BoardFileLoader::dataCharToType(char c) {
    switch (c) { //Map chars to types
        case '#':
            return Board::Tile::Type::wall;
            break;
        case '.':
            return Board::Tile::Type::coin;
        case ' ':
            break;
        default:
            throw FileLoaderException("board: unknown char in file");
            break;
    }

    return Board::Tile::Type::space;
}