#ifndef GHOSTCLYDE_H
#define GHOSTCLYDE_H

#include "GameLogic/Entities/Enemy.h"

class GhostClyde : public Enemy {
public:
    GhostClyde(const Transform & initial, const Position & scatterPos, unsigned int intelligence = 1, bool a = false);
    ~GhostClyde();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTCLYDE_H */