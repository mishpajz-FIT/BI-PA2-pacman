#ifndef GHOSTS_H
#define GHOSTS_H

#include "Enemy.h"

class GhostBlinky : Enemy {
public:
    GhostBlinky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostBlinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};

class GhostPinky : Enemy {
public:
    GhostPinky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostPinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};

class GhostInky : Enemy {
public:
    GhostInky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostInky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) override;
};

class GhostClyde : Enemy {
public:
    GhostClyde(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostClyde();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};

#endif /* GHOSTS_H */