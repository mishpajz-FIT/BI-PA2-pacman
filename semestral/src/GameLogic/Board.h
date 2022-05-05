#ifndef BOARD_H
#define BOARD_H

#include "Matrix.h"
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
    friend class BoardFileLoader;
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

public:

    Board();

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
     * @brief Get complementary position on opposite edge of board to position
     *
     * @param forPos position
     * @return Position complementary position
     */
    Position complementaryEdgePosition(const Position & forPos) const;

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

#endif /* BOARD_H */