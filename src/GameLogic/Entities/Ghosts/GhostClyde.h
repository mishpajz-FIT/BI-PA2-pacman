/****************************************************************
 * @file GhostClyde.h
 * @author Michal Dobes
 * @brief Model of enemy ghost "Clyde"
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef GHOSTCLYDE_H
#define GHOSTCLYDE_H

#include "GameLogic/Entities/Enemy.h"

/**
 * @brief Enemy ghost "Clyde"
 *
 * Ghost Clyde uses player's position as target if Clyde's distance to player is
 * more than 8 tiles, else Clyde uses it's scatter position.
 */
class GhostClyde : public Enemy {
public:

    /**
     * @brief Construct a new Ghost Clyde object
     *
     * @param initial Initial transform
     * @param scatterPos Scatter target
     * @param intelligence Intelligence level
     * @param a Alive
     */
    GhostClyde(
        const Transform & initial,
        const Position & scatterPos,
        unsigned int intelligence = 1,
        bool a = false);

    /**
     * @brief Destroy the Ghost Clyde object
     * 
     */
    ~GhostClyde();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTCLYDE_H */