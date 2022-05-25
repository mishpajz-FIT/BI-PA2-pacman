#include "GameLogic/Entities/Ghosts/GhostInky.h"

GhostInky::GhostInky(
    const Transform & initial,
    const Position & scatterPos,
    unsigned int intelligence,
    bool a) : Enemy(initial, scatterPos, a, intelligence) { }

GhostInky::~GhostInky() { }

Position GhostInky::calculateTarget(
    const Board & board,
    const Transform & playerTransform,
    const Position & specialPos) {

    Position newTarget(playerTransform.position);
    newTarget.movedBy(2, playerTransform.rotation);

    newTarget.x -= specialPos.x;
    newTarget.y -= specialPos.y;

    newTarget.x = 2 * abs(newTarget.x) + specialPos.x;
    newTarget.y = 2 * abs(newTarget.y) + specialPos.y;

    return newTarget;
}

std::pair<char, NCColors::ColorPairs> GhostInky::displayEntity() {
    if (frightened) {
        return Enemy::displayEntity();
    }
    return std::make_pair('&', NCColors::ColorPairs::ghostInky);
}