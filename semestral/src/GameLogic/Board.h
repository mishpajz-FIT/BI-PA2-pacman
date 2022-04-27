#ifndef BOARD_H
#define BOARD_H

#include "Matrix.h"
#include "Transform.h"
#include <string>

class Board {
public:
    enum class Tile {
        wall,
        floor,
        coin,
        cherry
    };

private:
    Matrix<Board::Tile> tiles;

    Position enemySpawn;
    Position playerSpawn;

public:
    Board(const std::string & file);

    Board::Tile tileAt(size_t x, size_t y) const;

    size_t getSizeX() const;
    size_t getSizeY() const;

    Position getEnemySpawn() const;
    Position getPlayerSpawn() const;

    bool interactWithTileAt(size_t x, size_t y);
};

#endif /* BOARD_H */