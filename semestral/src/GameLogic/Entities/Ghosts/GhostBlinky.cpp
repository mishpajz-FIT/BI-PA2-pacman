#include "GhostBlinky.h"

GhostBlinky::GhostBlinky(const Transform & initial, const Position & scatterPos, unsigned int intelligence, bool a) : Enemy(initial, scatterPos, a, intelligence) { }
GhostBlinky::~GhostBlinky() { }

Position GhostBlinky::calculateTarget(const Board & board, const Transform & playerTransform, const Position &) {
    return playerTransform.position;
}

std::pair<char, NCColors::ColorPairs> GhostBlinky::displayEntity() {
    if (frightened) {
        return Enemy::displayEntity();
    }
    return std::make_pair('&', NCColors::ColorPairs::ghostBlinky);
}