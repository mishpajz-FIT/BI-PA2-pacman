#include "Board.h"

#include <iostream>

Board::Board(const std::string & filePath) : tiles(1, 1), enemySpawn(-1, -1), playerSpawn(-1, -1) {
    loadBoardFromFile(filePath);
}

void Board::loadBoardFromFile(const std::string & filePath) {

    std::ifstream file(filePath);
    std::list<std::string> lines;

    if (!file.good()) {
        throw BoardWrongFile("board: error utilizing file");
    }

    std::string buffer;
    size_t lineLength = 0;

    while (std::getline(file, buffer)) { //Read lines from file

        if (lineLength == 0) { //If line is first with content set line length, else check if loaded line length is correct
            lineLength = buffer.length();
        } else if (lineLength != buffer.length()) {
            throw BoardInvalidGrid("board: wrong format of grid in file");
        }

        lines.emplace_back(buffer);
    }

    if (lines.size() == 0) {
        throw BoardInvalidGrid("board: empty grid");
    }

    buildTilesFromChars(lines); //Create Board::tiles from loaded lines

    buildCheckForCorrectEdges(); //Check if teleports are possible

    //TODO: REMOVE
    //Print out to console
    for (size_t y = 0; y < getSizeY(); y++) {
        for (size_t x = 0; x < getSizeX(); x++) {
            switch (tiles.at(x, y)) {
                case Board::Tile::Type::wall:
                    std::cout << 'W';
                    break;
                case Board::Tile::Type::coin:
                    std::cout << 'C';
                    break;
                case Board::Tile::Type::space:
                    std::cout << 'S';
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Player spawn: " << playerSpawn.x << "; " << playerSpawn.y << '\n';
    std::cout << "Enemy spawn: " << enemySpawn.x << "; " << enemySpawn.y << std::endl;
}

void Board::buildTilesFromChars(std::list<std::string> & lines) {
    Matrix<Board::Tile::Type> generatedTiles(lines.front().size(), lines.size());

    size_t y = 0;
    size_t x = 0;
    for (auto & l : lines) {
        for (auto & c : l) { //For each character in each line, check if is special then replace it by default, else convert it to correct type
            if (buildCheckForSpecialChars(c, x, y)) {
                generatedTiles.at(x++, y) = Board::Tile::defaultType();
            } else {
                generatedTiles.at(x++, y) = Board::Tile::dataCharToType(c);
            }
        }
        x = 0;
        y++;
    }

    if ((enemySpawn.x == -1 && enemySpawn.y == -1)
        || (playerSpawn.x == -1 && playerSpawn.y == -1)) { //Check if spawns have been set
        throw BoardInvalidGrid("board: missing spawn point");
    }

    tiles = generatedTiles;
}

bool Board::buildCheckForSpecialChars(char c, size_t x, size_t y) {
    switch (c) { //If chars are 'P' or 'E', set spawn positions (or throw is spawn has already been set)
        case 'P':
            if (playerSpawn.x == -1 && playerSpawn.y == -1) {
                playerSpawn = Position(x, y);
                return true;
            } else {
                throw BoardInvalidGrid("board: duplicit player spawn");
            }
            return true;
        case 'E':
            if (enemySpawn.x == -1 && enemySpawn.y == -1) {
                enemySpawn = Position(x, y);
            } else {
                throw BoardInvalidGrid("board: duplicit player spawn");
            }
            return true;
        default:
            break;
    }
    return false;
}

void Board::buildCheckForCorrectEdges() {

    for (size_t x = 1; x < getSizeX() - 1; x++) { //Check if tiles at top and bottom are same
        if (tiles.at(x, 0) != tiles.at(x, getSizeY() - 1)) {
            throw BoardInvalidGrid("board: incorrect teleport");
        }
    }

    for (size_t y = 1; y < getSizeY() - 1; y++) { //Check if tiles at left and right are same
        if (tiles.at(0, y) != tiles.at(getSizeX() - 1, y)) {
            throw BoardInvalidGrid("board: incorrect teleport");
        }
    }
}

bool Board::isTileCoordinateValid(const Position & pos) const {
    if ((unsigned)(pos.x) >= tiles.getSizeX() || (unsigned)(pos.y) >= tiles.getSizeY()) {
        return false;
    }
    return true;
}

bool Board::isTileCrossroad(const Position & pos) const {
    if (!isTileAllowingMovement(pos)) {
        return false;
    }

    size_t numberOfPaths = 0;
    for (size_t d = 0; d < 4; d++) {
        if (isTileAllowingMovement(Transform(pos, Rotation(d)).moveByOne().position)) {
            if (++numberOfPaths >= 3) {
                return true;
            }
        }
    }

    return false;

}

bool Board::isTileEdge(const Position & pos) const {
    if ((size_t)(pos.x) == (getSizeX() - 1) || pos.x == 0
        || (size_t)(pos.y) == (getSizeY() - 1) || pos.y == 0) {
        return true;
    }
    return false;
}

bool Board::isTileAllowingMovement(const Position & pos) const {
    try {
        if (Board::Tile::typeAllowsMovement(tileAt(pos))) {
            return true;
        }
    }
    catch (BoardException & e) { }
    return false;
}

Board::Tile::Type Board::tileAt(const Position & pos) const {
    try {
        return tiles.at(pos.x, pos.y);
    }
    catch (std::out_of_range & e) {
        throw BoardException("board: tileAt - coordinates are out of range");
    }
}

size_t Board::getSizeX() const {
    return tiles.getSizeX();
}

size_t Board::getSizeY() const {
    return tiles.getSizeY();
}

Position Board::getEnemySpawn() const {
    return enemySpawn;
}

Position Board::getPlayerSpawn() const {
    return playerSpawn;
}

bool Board::interactWithTileAt(const Position & pos) {

    if (Board::Tile::typeAllowsInteraction(tileAt(pos))) {
        tiles.at(pos.x, pos.y) = Board::Tile::defaultType();
        return true;
    }

    return false;
}

Board::Tile::Type Board::Tile::dataCharToType(char c) {
    switch (c) { //Map chars to types
        case '#':
            return Board::Tile::Type::wall;
            break;
        case '.':
            return Board::Tile::Type::coin;
        case ' ':
            break;
        default:
            throw BoardInvalidGrid("board: unknown char in file");
            break;
    }

    return Board::Tile::Type::space;
}

Board::Tile::Type Board::Tile::defaultType() {
    return Board::Tile::Type::wall;
}

bool Board::Tile::typeAllowsMovement(const Board::Tile::Type & t) {
    switch (t) {
        case Board::Tile::Type::coin:
        case Board::Tile::Type::space:
            return true;
        default:
            break;
    }
    return false;
}

bool Board::Tile::typeAllowsInteraction(const Type & t) {
    switch (t) {
        case Board::Tile::Type::coin:
            return true;
        default:
            break;
    }
    return false;
}

BoardException::BoardException(const std::string & message) : runtime_error(message) { };
BoardWrongFile::BoardWrongFile(const std::string & message) : BoardException(message) { };
BoardInvalidGrid::BoardInvalidGrid(const std::string & message) : BoardException(message) { };
