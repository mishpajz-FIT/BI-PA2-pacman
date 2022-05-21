#include "GhostClyde.h"

GhostClyde::GhostClyde(const Transform & initial, const Position & scatterPos, unsigned int intelligence, bool a) : Enemy(initial, scatterPos, a, intelligence) { }
GhostClyde::~GhostClyde() { }

Position GhostClyde::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    if (Position::distanceBetween(playerTransform.position, transform.position) > 8.0) {
        return playerTransform.position;
    }
    return scatterTarget;
}

std::pair<char, NCColors::ColorPairs> GhostClyde::displayEntity() {
    if (frightened) {
        return Enemy::displayEntity();
    }
    return std::make_pair('&', NCColors::ColorPairs::ghostClyde);
}