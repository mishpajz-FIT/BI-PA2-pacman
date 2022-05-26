#include "GameLogic/Board.h"

//SECTION: Board::Tile
Board::Tile::Type Board::Tile::defaultType() {
    return Board::Tile::Type::space;
}

bool Board::Tile::typeAllowsMovement(const Board::Tile::Type & t) {
    switch (t) {
        case Board::Tile::Type::coin:
        case Board::Tile::Type::space:
        case Board::Tile::Type::bonus:
        case Board::Tile::Type::frighten:
            return true;
        default:
            break;
    }
    return false;
}

bool Board::Tile::typeAllowsInteraction(const Type & t) {
    switch (t) {
        case Board::Tile::Type::coin:
        case Board::Tile::Type::bonus:
        case Board::Tile::Type::frighten:
            return true;
        default:
            break;
    }
    return false;
}

std::pair<char, NCColors::ColorPairs> Board::Tile::typeDisplay(const Type & t) {
    char c = ' ';
    NCColors::ColorPairs color = NCColors::ColorPairs::basic;

    switch (t) {
        case Board::Tile::Type::wall:
            color = NCColors::ColorPairs::wall;
            break;
        case Board::Tile::Type::space:
            color = NCColors::ColorPairs::space;
            break;
        case Board::Tile::Type::coin:
            color = NCColors::ColorPairs::coin;
            c = '.';
            break;
        case Board::Tile::Type::frighten:
            color = NCColors::ColorPairs::coin;
            c = 'o';
            break;
        case Board::Tile::Type::bonus:
            color = NCColors::ColorPairs::bonus;
            c = '*';
            break;
    }

    return std::make_pair(c, color);
}
//!SECTION

Board::Board() : tiles(1, 1), enemySpawn(-1, -1), playerSpawn(-1, -1), numberOfCoins(0) { }

Board::Board(
    const Matrix<Board::Tile::Type> & newTiles,
    const Position & newEnemySpawn,
    const Position & newPlayersSpawn)
    :
    tiles(newTiles),
    enemySpawn(newEnemySpawn),
    playerSpawn(newPlayersSpawn),
    numberOfCoins(0) {

    if (!isTileCoordinateValid(playerSpawn) || !isTileCoordinateValid(enemySpawn)) {
        throw std::invalid_argument("Board: Board - invalid enemy or player spawn");
    }

    // Count amount of coins in board
    for (size_t y = 0; y < newTiles.getSizeY(); y++) {
        for (size_t x = 0; x < newTiles.getSizeX(); x++) {
            if (newTiles.at(x, y) == Board::Tile::Type::coin) {
                numberOfCoins++;
            }
        }
    }
}

Board::Tile::Type Board::tileAt(const Position & pos) const {
    try {
        return tiles.at(pos.x, pos.y);
    }
    catch (std::out_of_range & e) {
        throw BoardException("board: tileAt - coordinates are out of range");
    }
}

bool Board::isTileCoordinateValid(const Position & pos) const {
    if ((size_t)(pos.x) >= tiles.getSizeX() || (size_t)(pos.y) >= tiles.getSizeY()) {
        return false;
    }
    return true;
}

bool Board::isTileCrossroad(const Position & pos) const {
    if (!isTileAllowingMovement(pos)) {
        return false;
    }

    // Check in each direction around position, if three or more tiles around
    // tile allow for movement, tile is crossroad
    size_t numberOfPaths = 0;
    for (size_t d = 0; d < 4; d++) {
        if (isTileAllowingMovement(pos.movedBy(1, Rotation(d)))) {
            if (++numberOfPaths >= 3) {
                return true;
            }
        }
    }

    return false;

}

bool Board::isTileEdge(const Position & pos) const {
    //Tile needs to have position coordinates as 0 or board size minus one
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

Position Board::complementaryEdgePosition(Position forPos) const {
    // If coordinate is at edge, get change coordinate to opposite

    if (forPos.x == 0) {
        forPos.x = (getSizeX() - 1);
    } else if ((unsigned long)(forPos.x) >= (getSizeX() - 1)) {
        forPos.x = 0;
    }

    if (forPos.y == 0) {
        forPos.y = (getSizeY() - 1);
    } else if ((unsigned long)(forPos.y) >= (getSizeY() - 1)) {
        forPos.y = 0;
    }

    return forPos;
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
        if (tileAt(pos) == Board::Tile::Type::coin) { //< If coin will be removed,
        // decrease the number of coins
            numberOfCoins--;
        }
        tiles.at(pos.x, pos.y) = Board::Tile::defaultType();
        return true;
    }

    return false;
}

unsigned int Board::getNumberOfCoins() {
    return numberOfCoins;
}

std::optional<Position> Board::placeBonusTile() {
    // Five tries to place the bonus, else return as unsuccessfull
    for (unsigned int i = 0; i < 5; i++) {
        // Get random position
        srand(time(0));
        size_t randX = rand() % (getSizeX());
        size_t randY = rand() % (getSizeY());
        Position tilePos(randX, randY);

        // Place bonus only if tile is of type default
        if (tileAt(tilePos) == Tile::defaultType()) {
            tiles.at(randX, randY) = Tile::Type::bonus;
            return tilePos;
        }
    }
    return { };
}

BoardException::BoardException(const std::string & message) : runtime_error(message) { }
