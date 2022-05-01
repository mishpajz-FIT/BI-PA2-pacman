#ifndef BOARD_H
#define BOARD_H

#include "../Structures/Matrix.h"
#include "Transform.h"
#include <string>
#include <stdexcept>
#include <list>
#include <fstream>

/**
 * @brief Game Board
 *
 */
class Board {
public:
    /**
     * @brief Tile of Board
     *
     */
    struct Tile {
        /**
         * @brief Type of tile
         *
         */
        enum class Type {
            wall,
            space,
            coin
        };

        /**
         * @brief Imported file char to type conversion
         *
         * @exception BoardInvalidGrid char is unknown
         *
         * @param c char from file
         * @return Type Tile::Type the inputted char represents
         */
        static Type dataCharToType(char c);
    };

private:
    Matrix<Board::Tile::Type> tiles; //< Tiles of board stored in a matrix

    Position enemySpawn; //< Position in maze of enemy spawn
    Position playerSpawn; //< Position in maze of player spawn

    /**
     * @brief Load tiles from file
     *
     * @param filename Path to file
     */
    void loadBoardFromFile(const std::string & filename);

    /**
     * @brief Convert lines from input file into Board::tiles matrix
     * @exception BoardInvalidGrid missing spawn point/unknown char/duplicit spawn/edges are invalid/wrong format
     * @exception BoardWrongFile file couldn't be read
     *
     * @param lines list of lines
     */
    void buildTilesFromChars(std::list<std::string> & lines);

    /**
     * @brief Check if char from input file has special meaning and perform action associated with it
     *
     * If char has special meaning, it probably should not be propagated further the board creation (or should be changed).
     *
     * Could signalize player or enemy spawn etc.
     *
     * @exception BoardInvalidGrid duplicit spawn
     *
     * @param c Char to check
     * @param x coordinate X
     * @param y coordinate Y
     * @return true Char has special meaning
     * @return false Char has no special meaning
     */
    bool buildCheckForSpecialChars(char c, size_t x, size_t y);

    /**
     * @brief Check if edges of Board::tiles are correct
     *
     * Edges must be same on both sides.
     *
     * @exception BoardInvalidGrid edges are invalid
     */
    void buildCheckForCorrectEdges();

public:
    /**
     * @brief Check if tile with coordinate is in Board::tiles
     *
     * @param x coordinate X
     * @param y coordinate Y
     * @return true Coordiante is in Board::tiles
     * @return false Coordinate is not in Board::tiles
     */
    bool isTileCoordinateValid(size_t x, size_t y) const;

    /**
     * @brief Check if tile with position is in Board::tiles
     *
     * @param pos position of tile
     * @return true Position is in Board::tiles
     * @return false Position is not in Board::tiles
     */
    bool isTileCoordinateValid(const Position & pos) const;

    /**
     * @brief Construct a new Board object from file
     *
     * File needs to have correct format.
     *
     * @exception BoardInvalidGrid wrong file format
     * @exception BoardWrongFile couldn't read file
     *
     * @param filePath path to file
     */
    Board(const std::string & filePath);

    /**
     * @brief Get tile in board at coordinate
     *
     * @exception BoardException wrong coordinates
     *
     * @param x coordinate X
     * @param y coordinate Y
     * @return Board::Tile::Type Type of tile at coordinates
     */
    Board::Tile::Type tileAt(size_t x, size_t y) const;

    /**
     * @brief Get tile in board at position
     *
     * @exception BoardException wrong position
     *
     * @param pos position of tile
     * @return Board::Tile::Type Type of tile at coordinates
     */
    Board::Tile::Type tileAt(const Position & pos) const;

    /**
     * @brief Get size of board in X dimension
     *
     * @return size_t size in X dimension
     */
    size_t getSizeX() const;

    /**
     * @brief Get size of board in Y dimension
     *
     * @return size_t size in Y dimension
     */
    size_t getSizeY() const;

    /**
     * @brief Get position of enemy spawn
     *
     * @return Position position of enemy spawn
     */
    Position getEnemySpawn() const;

    /**
     * @brief Get position of player spawn
     *
     * @return Position position of player spawn
     */
    Position getPlayerSpawn() const;

    /**
     * @brief Interact with tile in board
     *
     * If action for tile is defined, replaces it with Board::Tile::Type::wall
     *
     * @exception BoardException wrong coordinates
     *
     * @param x coordinate X
     * @param y coordinate Y
     * @return true Tile was interractable
     * @return false Tile was not interractable
     */
    bool interactWithTileAt(size_t x, size_t y);
};

/**
 * @brief Runtime Board exception
 *
 */
struct BoardException : public std::runtime_error {
    BoardException(const std::string & message);
};

/**
 * @brief Runtime Board exception signaling problem with input file
 *
 */
struct BoardWrongFile : public BoardException {
    BoardWrongFile(const std::string & message);
};

/**
 * @brief Runtime Board exception signaling problem with loading Board from input file
 *
 */
struct BoardInvalidGrid : public BoardException {
    BoardInvalidGrid(const std::string & message);
};

#endif /* BOARD_H */