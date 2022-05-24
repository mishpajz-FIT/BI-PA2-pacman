#ifndef GHOSTBLINKY_H
#define GHOSTBLINKY_H

#include "GameLogic/Entities/Enemy.h"

class GhostBlinky : public Enemy {
public:
    GhostBlinky(const Transform & initial, const Position & scatterPos, unsigned int intelligence = 1, bool a = false);
    ~GhostBlinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTBLINKY_H */