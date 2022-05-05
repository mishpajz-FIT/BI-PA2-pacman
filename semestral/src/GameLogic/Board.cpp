#include "Board.h"

#include <iostream>

Board::Board() : tiles(1, 1), enemySpawn(-1, -1), playerSpawn(-1, -1) { }

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

Position Board::complementaryEdgePosition(const Position & forPos) const {
    Position newPos(forPos);

    if ((size_t)(forPos.x) == (getSizeX() - 1)) {
        newPos.x = 0;
    } else if ((size_t)(forPos.x) == 0) {
        newPos.x = (getSizeX() - 1);
    }

    if ((size_t)(forPos.y) == (getSizeY() - 1)) {
        newPos.y = 0;
    } else if ((size_t)(forPos.y) == 0) {
        newPos.y = (getSizeY() - 1);
    }

    return newPos;
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
