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
    if (frightened || scatter) {
        return Enemy::calculateTarget(board, playerTransform);
    }

    Position newTarget(playerTransform.position);
    newTarget.movedBy(2, playerTransform.rotation); //< Move player's position in its direction by 2 tiles
    Position playerMoved = newTarget;

    // Get vector from newTarget to specialPos
    newTarget.x -= specialPos.x;
    newTarget.y -= specialPos.y;

    // Add 2 times the vector from newTarget to specialPos to player's position
    newTarget.x = playerMoved.x + 2 * newTarget.x;
    newTarget.y = playerMoved.y + 2 * newTarget.y;

    return newTarget;
}

std::pair<char, NCColors::ColorPairs> GhostInky::displayEntity() {
    if (frightened) {
        return Enemy::displayEntity();
    }
    return std::make_pair('&', NCColors::ColorPairs::ghostInky);
}