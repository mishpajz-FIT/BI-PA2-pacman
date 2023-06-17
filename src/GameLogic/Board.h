/****************************************************************
 * @file Board.h
 * @author Michal Dobes
 * @brief Game board model
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <tuple>
#include <string>
#include <stdexcept>
#include <list>
#include <fstream>
#include <optional>

#include "Utilities/NCColors.h"
#include "Structures/Transforms/Transform.h"
#include "Structures/Matrix.h"

/**
 * @brief Game Board
 *
 * Manages playing board (map) for game.
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
            coin,
            frighten,
            bonus
        };

        /**
         * @brief Default tile type
         *
         * Simplest type of tile
         *
         * @return Type
         */
        static Type defaultType();

        /**
         * @brief Check if type allows movement
         *
         * @param t type of tile
         * @return true
         * @return false
         */
        static bool typeAllowsMovement(const Type & t);

        /**
         * @brief Check if type allows interaction
         *
         * @param t type of tile
         * @return true
         * @return false
         */
        static bool typeAllowsInteraction(const Type & t);

        /**
         * @brief Get display information used for displaying the tile
         *
         * Gets char and color information
         *
         * @param t type of tile
         * @return std::pair<char, NCColors::ColorPairs> pair with char that represents entity and color pair
         */
        static std::pair<char, NCColors::ColorPairs> typeDisplay(const Type & t);
    };

private:
    Matrix<Board::Tile::Type> tiles; //< Tiles of board stored in a matrix

    Position enemySpawn; //< Position in maze of enemy spawn
    Position playerSpawn; //< Position in maze of player spawn

    unsigned int numberOfCoins; //< Current number of Coin tiles in board

public:

    /**
     * @brief Construct a new, empty Board object
     *
     */
    Board();

    /**
     * @brief Construct a new Board object
     *
     * @param newTiles Matrix of tiles
     * @param newEnemySpawn Position of enemy spawn
     * @param newPlayersSpawn Position of player spawn
     */
    Board(
        const Matrix<Board::Tile::Type> & newTiles,
        const Position & newEnemySpawn,
        const Position & newPlayersSpawn);

    /**
     * @brief Get tile at position in board
     *
     * @exception BoardException wrong position
     *
     * @param pos position of tile
     * @return Board::Tile::Type
     */
    Board::Tile::Type tileAt(const Position & pos) const;

    /**
     * @brief Check if tile with position is in board
     *
     * @param pos position of tile
     * @return true
     * @return false
     */
    bool isTileCoordinateValid(const Position & pos) const;

    /**
     * @brief Check if tile at position is a crossroad
     *
     * To be a crossroad, tile needs to have at least 3 tiles in vicinity that allow for movement.
     *
     * @param pos position of tile to check
     * @return true
     * @return false
     */
    bool isTileCrossroad(const Position & pos) const;

    /**
     * @brief Check if tile at position is at edge of board
     *
     * @param pos position of tile to check
     * @return true
     * @return false
     */
    bool isTileEdge(const Position & pos) const;

    /**
     * @brief Check if tile at position is allowing movement
     *
     * @param pos position of tile to check
     * @return true tile is allowing movement
     * @return false tile is not allowing movement
     */
    bool isTileAllowingMovement(const Position & pos) const;

    /**
     * @brief Get complementary position on opposite edge of board to position
     *
     * Position needs to be at edge of board, else original position is returned.
     *
     * @param forPos position
     * @return Position complementary position
     */
    Position complementaryEdgePosition(Position forPos) const;

    /**
     * @brief Get the size of board in X dimension
     *
     * @return size_t size in X dimension
     */
    size_t getSizeX() const;

    /**
     * @brief Get the size of board in Y dimension
     *
     * @return size_t size in Y dimension
     */
    size_t getSizeY() const;

    /**
     * @brief Get the position of enemy spawn
     *
     * @return Position position of enemy spawn
     */
    Position getEnemySpawn() const;

    /**
     * @brief Get the position of player spawn
     *
     * @return Position position of player spawn
     */
    Position getPlayerSpawn() const;

    /**
     * @brief Interact with tile in board
     *
     * If tile is allowing interactions, replaces it with default tile type
     *
     * @exception BoardException wrong coordinates
     *
     * @param pos position of tile
     * @return true Tile was interactable
     * @return false Tile is not interactable
     */
    bool interactWithTileAt(const Position & pos);

    /**
     * @brief Get current number of coins in board
     *
     * @return unsigned int
     */
    unsigned int getNumberOfCoins();

    /**
     * @brief Place bonus tile at random position in board which has default tile
     *
     * @return std::optional<Position> Empty if bonus couldn't be placed, else
     *      position to which bonus was placed
     */
    std::optional<Position> placeBonusTile();
};

/**
 * @brief Runtime Board exception
 *
 */
struct BoardException : public std::runtime_error {
    BoardException(const std::string & message);
};

#endif /* BOARD_H */