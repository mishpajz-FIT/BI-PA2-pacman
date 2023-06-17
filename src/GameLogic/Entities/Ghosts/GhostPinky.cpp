#include "GameLogic/Entities/Ghosts/GhostPinky.h"

GhostPinky::GhostPinky(
    const Transform & initial,
    const Position & scatterPos,
    unsigned int intelligence,
    bool a) : Enemy(initial, scatterPos, a, intelligence) { }
GhostPinky::~GhostPinky() { }

Position GhostPinky::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    if (frightened || scatter) {
        return Enemy::calculateTarget(board, playerTransform);
    }

    Position newTarget(playerTransform.position);
    newTarget.moveBy(4, playerTransform.rotation);
    if (playerTransform.rotation.direction == Rotation::Direction::up) {
        newTarget.moveBy(4, Rotation(Rotation::Direction::left));
    }
    return newTarget;
}

std::pair<char, NCColors::ColorPairs> GhostPinky::displayEntity() {
    if (frightened) {
        return Enemy::displayEntity();
    }
    return std::make_pair('&', NCColors::ColorPairs::ghostPinky);
}