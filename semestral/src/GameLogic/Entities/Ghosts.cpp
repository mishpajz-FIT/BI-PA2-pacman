#include "Ghosts.h"

//SECTION: GhostBlinky
GhostBlinky::GhostBlinky(const Transform & initial, const Position & scatterPos, bool a) : Enemy(initial, scatterPos, a) { }
GhostBlinky::~GhostBlinky() { };

Position GhostBlinky::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    return playerTransform.position;
}
//!SECTION


//SECTION: GhostPinky
GhostPinky::GhostPinky(const Transform & initial, const Position & scatterPos, bool a) : Enemy(initial, scatterPos, a) { }
GhostPinky::~GhostPinky() { };

Position GhostPinky::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    Position newTarget(playerTransform.position);
    newTarget.moveBy(4, playerTransform.rotation);
    if (playerTransform.rotation.direction == Rotation::Direction::up) {
        newTarget.moveBy(4, Rotation(Rotation::Direction::left));
    }
    return newTarget;
}
//!SECTION


//SECTION: GhostInky
GhostInky::GhostInky(const Transform & initial, const Position & scatterPos, bool a) : Enemy(initial, scatterPos, a) { }
GhostInky::~GhostInky() { };

Position GhostInky::calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    Position newTarget(playerTransform.position);
    newTarget.movedBy(2, playerTransform.rotation);

    newTarget.x -= specialPos.x;
    newTarget.y -= specialPos.y;

    newTarget.x = 2 * abs(newTarget.x) + specialPos.x;
    newTarget.y = 2 * abs(newTarget.y) + specialPos.y;

    return newTarget;
}
//!SECTION


//SECTION: GhostClyde
GhostClyde::GhostClyde(const Transform & initial, const Position & scatterPos, bool a) : Enemy(initial, scatterPos, a) { }
GhostClyde::~GhostClyde() { };

Position GhostClyde::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    if (Position::distanceBetween(playerTransform.position, transform.position) > 8.0) {
        return playerTransform.position;
    }
    return scatterTarget;
}
//!SECTION
