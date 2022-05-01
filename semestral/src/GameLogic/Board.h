#ifndef BOARD_H
#define BOARD_H

#include "../Structures/Matrix.h"
#include "Transform.h"
#include <string>
#include <stdexcept>
#include <list>

class Board {
public:
    struct Tile {
        enum class Type {
            wall,
            space,
            coin
        };

        static Type dataCharToType(char c);
        static char typeToDisplayChar(Type t);
    };

private:
    Matrix<Board::Tile::Type> tiles;

    Position enemySpawn;
    Position playerSpawn;

    bool isTileCoordinateValid(size_t x, size_t y) const;

    void loadBoardFromFile(const std::string & filename);
    void buildTilesFromChars(std::list<std::string> & lines);
    bool checkForSpecialChars(char c, size_t x, size_t y);

public:
    Board(const std::string & filePath);

    Board::Tile::Type tileAt(size_t x, size_t y) const;

    size_t getSizeX() const;
    size_t getSizeY() const;

    Position getEnemySpawn() const;
    Position getPlayerSpawn() const;

    bool interactWithTileAt(size_t x, size_t y);
};

struct BoardException : public std::runtime_error {
    BoardException(const std::string & message);
};

struct BoardWrongFile : public BoardException {
    BoardWrongFile(const std::string & message);
};

struct BoardInvalidGrid : public BoardException {
    BoardInvalidGrid(const std::string & message);
};

#endif /* BOARD_H */