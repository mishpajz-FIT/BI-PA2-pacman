#ifndef GHOSTINKY_H
#define GHOSTINKY_H

#include "GameLogic/Entities/Enemy.h"

class GhostInky : public Enemy {
public:
    GhostInky(const Transform & initial, const Position & scatterPos, unsigned int intelligence = 1, bool a = false);
    ~GhostInky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTINKY_H */