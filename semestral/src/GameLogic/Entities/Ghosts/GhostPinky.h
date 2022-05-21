#ifndef GHOSTPINKY_H
#define GHOSTPINKY_H

#include "Enemy.h"

class GhostPinky : public Enemy {
public:
    GhostPinky(const Transform & initial, const Position & scatterPos, unsigned int intelligence = 1, bool a = false);
    ~GhostPinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* GHOSTPINKY_H */