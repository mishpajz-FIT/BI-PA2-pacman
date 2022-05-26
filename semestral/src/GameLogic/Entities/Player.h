/****************************************************************
 * @file Player.h
 * @author Michal Dobes
 * @brief Model of player entity
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "GameLogic/Entities/Entity.h"
#include "GameLogic/Board.h"

/**
 * @brief Player entity
 *
 */
class Player : public Entity {
private:

    /**
     * @brief Change transform if tile in board at transform's position allows it
     *
     * Tile at transform's position needs to allow movement
     *
     * @param to Transform which should be set as Player's transform
     * @param board Board in which Player is moving in
     * @return true Transform was changed
     * @return false Tile in board is not allowing movement to this transform
     */
    bool ifPossibleChangeTransform(const Transform & to, const Board & board);

public:

    /**
     * @brief Construct a Player
     *
     * @param initial Initial transform
     * @param a Alive
     */
    Player(const Transform & initial, bool a = true);

    /**
     * @brief Move in direction based on next rotation
     *
     * @param board Board in which Player is moving in
     */
    void move(const Board & board);

    /**
     * @brief Change next rotation
     *
     * @param to Next rotation (in which to move)
     */
    void rotate(const Rotation & to);

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* PLAYER_H */