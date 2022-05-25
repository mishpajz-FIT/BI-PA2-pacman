/****************************************************************
 * @file GhostBlinky.h
 * @author Michal Dobes
 * @brief Model of enemy ghost "Blinky"
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
*****************************************************************/

#ifndef GHOSTBLINKY_H
#define GHOSTBLINKY_H

#include "GameLogic/Entities/Enemy.h"

/**
 * @brief Enemy ghost "Blinky"
 *
 * Ghost Blinky uses player's position as it's target.
 */
class GhostBlinky : public Enemy {
public:

    /**
     * @brief Construct a new Ghost Blinky object
     *
     * @param initial Initial transform
     * @param scatterPos Scatter target
     * @param intelligence Intelligence level
     * @param a Alive
     */
    GhostBlinky(
        const Transform & initial,
        const Position & scatterPos,
        unsigned int intelligence = 1,
        bool a = false);

    /**
     * @brief Destroy the Ghost Blinky object
     *
     */
    ~GhostBlinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTBLINKY_H */