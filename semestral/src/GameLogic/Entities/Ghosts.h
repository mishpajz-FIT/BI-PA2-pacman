#ifndef GHOSTS_H
#define GHOSTS_H

#include "Enemy.h"

//SECTION: GhostBlinky
class GhostBlinky : Enemy {
public:
    GhostBlinky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostBlinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};
//!SECTION


//SECTION: GhostPinky
class GhostPinky : Enemy {
public:
    GhostPinky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostPinky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};
//!SECTION


//SECTION: GhostInky
class GhostInky : Enemy {
public:
    GhostInky(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostInky();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) override;
};
//!SECTION


//SECTION: GhostClyde
class GhostClyde : Enemy {
public:
    GhostClyde(const Transform & initial, const Position & scatterPos, bool a = false);
    ~GhostClyde();

    Position calculateTarget(const Board & board, const Transform & playerTransform, const Position &) override;
};
//!SECTION

#endif /* GHOSTS_H */