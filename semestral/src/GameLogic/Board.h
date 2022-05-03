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

        /**
         * @brief Default tile type
         *
         * Simplest type of tile
         *
         * @return Type default type
         */
        static Type defaultType();

        /**
         * @brief Check if type allows movement
         *
         * @param t type of tile
         * @return true type allows movement
         * @return false type doesnt allow movement
         */
        static bool typeAllowsMovement(const Type & t);

        /**
         * @brief Check if type allows interaction
         *
         * @param t type of tile
         * @return true type allows interaction
         * @return false type doesnt allow interaction
         */
        static bool typeAllowsInteraction(const Type & t);
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
     * @brief Check if tile with position is in Board::tiles
     *
     * @param pos position of tile
     * @return true Position is in Board::tiles
     * @return false Position is not in Board::tiles
     */
    bool isTileCoordinateValid(const Position & pos) const;

    /**
     * @brief Check if tile is crossroad
     *
     * To be a crossroad, tile needs to have at least 3 tiles in vicinity that allow for movement.
     *
     * @param pos position of tile to check
     * @return true tile is crossroad
     * @return false tile is not crossroad
     */
    bool isTileCrossroad(const Position & pos) const;

    /**
     * @brief Check if tile is at edge of board
     *
     * @param pos position of tile to check
     * @return true tile is at edge of board
     * @return false tile is not at edge of board
     */
    bool isTileEdge(const Position & pos) const;

    /**
     * @brief Check if type of tile is allowing movement
     *
     * @param pos position of tile to check
     * @return true tile is allowing movement
     * @return false tile is not allowing movement
     */
    bool isTileAllowingMovement(const Position & pos) const;

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
     * @param pos position of tile
     * @return true Tile was interractable
     * @return false Tile was not interractable
     */
    bool interactWithTileAt(const Position & pos);
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