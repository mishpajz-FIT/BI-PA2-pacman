/****************************************************************
 * @file GhostInky.h
 * @author Michal Dobes
 * @brief Model of enemy ghost "Inky"
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef GHOSTINKY_H
#define GHOSTINKY_H

#include "GameLogic/Entities/Enemy.h"

/**
 * @brief Enemy ghost "Inky"
 *
 * Ghost Inky uses player's position and some other entity's position to calculate
 * its target. Inky's target lies on a straight line that connects the player
 * and the other entity, so that the other entity's position is exactly halfway
 * between Inky's target position and the player.
 */
class GhostInky : public Enemy {
public:

    /**
     * @brief Construct a new Ghost Inky object
     *
     * @param initial Initial transform
     * @param scatterPos Scatter target
     * @param intelligence Intelligence level
     * @param a Alive
     */
    GhostInky(
        const Transform & initial,
        const Position & scatterPos,
        unsigned int intelligence = 1,
        bool a = false);

    /**
     * @brief Destroy the Ghost Inky object
     * 
     */
    ~GhostInky();

    Position calculateTarget(
        const Board & board,
        const Transform & playerTransform,
        const Position & specialPos) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTINKY_H */