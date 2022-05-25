/****************************************************************
 * @file GhostPinky.h
 * @author Michal Dobes
 * @brief Model of enemy ghost "Pinky"
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GHOSTPINKY_H
#define GHOSTPINKY_H

#include "GameLogic/Entities/Enemy.h"

/**
 * @brief Enemy ghost "Pinky"
 *
 * Ghost Pinky's target position is 4 tiles in front of player. If player is
 * heading up, then Pinky's target position is shifted additional 4 tiles to left.
 */
class GhostPinky : public Enemy {
public:

    /**
     * @brief Construct a new Ghost Pinky object
     *
     * @param initial Initial transform
     * @param scatterPos Scatter target
     * @param intelligence Intelligence level
     * @param a Alive
     */
    GhostPinky(
        const Transform & initial,
        const Position & scatterPos,
        unsigned int intelligence = 1,
        bool a = false);

    /**
     * @brief Destroy the Ghost Pinky object
     * 
     */
    ~GhostPinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTPINKY_H */