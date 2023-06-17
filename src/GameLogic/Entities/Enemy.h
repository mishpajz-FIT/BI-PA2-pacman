/****************************************************************
 * @file Enemy.h
 * @author Michal Dobes
 * @brief Model of enemy
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "GameLogic/Entities/Entity.h"
#include "GameLogic/Board.h"

/**
 * @brief Enemy entity
 *
 */
class Enemy : public Entity {
protected:
    unsigned int intelligence; //< Intelligence level

    // Current states of enemy
    bool frightened;
    bool scatter;

    Rotation currentDirection; //< Enemy's transform rotation reflects rotation
    // that has already happend, currentDirection is rotation that will happen
    // next

    Position scatterTarget; //< Target when in scatter state

    /**
     * @brief Calculate next direction of movement
     *
     * Changes nextRotation, based on tile that is pointed to by currentDirection
     *
     * Flowchart of steps used in calculating next direction:
     * @image html nextdirection.png
     *
     * @param board Board in which is this enemy moving in
     * @param target Target to which this enemy wants to move to
     */
    void calculateNextDirection(const Board & board, const Position & target);

    /**
     * @brief Calculate target to which this enemy wants to move to
     *
     * @param board Board in which is this enemy moving in
     * @param playerTransform Transform of player
     * @param specialPos Special position to use in calculation
     * @return Position
     */
    virtual Position calculateTarget(
        const Board & board,
        const Transform & playerTransform,
        const Position & specialPos = Position());

public:

    /**
     * @brief Construct a new Enemy object
     *
     * @param initial Initial transform
     * @param scatterPos Target in scatter mode
     * @param a Is alive
     * @param intelligenceLevel Level of intelligence
     */
    Enemy(const Transform & initial, const Position & scatterPos, bool a = false, unsigned int intelligenceLevel = 1);

    /**
     * @brief Destroy the Enemy object
     *
     */
    virtual ~Enemy();

    /**
     * @brief Move this enemy and calculate next direction of movement
     *
     * @param board Board in which is this enemy moving in
     * @param playerTransform Transform of player
     * @param specialPos Special position to use in calculation of target
     */
    void move(const Board & board, const Transform & playerTransform, const Position & specialPos = Position());

    /**
     * @brief Toggle on/off scatter mode
     *
     * @param board Board in which is this enemy moving in
     */
    void toggleScatter(const Board & board);

    /**
     * @brief Togle on/off frighten mode
     *
     * @param board Board in which is this enemy moving in
     */
    void toggleFrighten(const Board & board);

    /**
     * @brief Is this enemy in frighten mode
     *
     * @return true
     * @return false
     */
    bool isFrightened();

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* ENEMY_H */