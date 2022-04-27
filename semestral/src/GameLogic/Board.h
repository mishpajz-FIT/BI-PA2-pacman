#ifndef BOARD_H
#define BOARD_H

#include "Matrix.h"
#include "Transform.h"
#include <string>

class Board {
public:
    struct Tile {
        enum class Type {
            wall,
            floor,
            coin,
            cherry
        };

        static Type dataCharToType(char c);
        static char typeToDisplayChar(Type t);
    };

private:
    Matrix<Board::Tile> tiles;

    Position enemySpawn;
    Position playerSpawn;

    bool isTileCoordinateValid(size_t x, size_t y);

public:
    Board(const std::string & file);

    Board::Tile::Type tileAt(size_t x, size_t y) const;

    size_t getSizeX() const;
    size_t getSizeY() const;

    Position getEnemySpawn() const;
    Position getPlayerSpawn() const;

    bool interactWithTileAt(size_t x, size_t y);
};

#endif /* BOARD_H */