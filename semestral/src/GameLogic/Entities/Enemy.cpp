#include "Enemy.h"

void Enemy::calculateNextDirection(const Board & board, const Position & target) {
    Position nextTilePos = transform.position.movedBy(1, currentDirection);
    if (board.isTileCrossroad(nextTilePos)) {
        std::vector<std::pair<double, Rotation>> distances;

        for (size_t d = 0; d < 4; d++) {
            Position calculatePosition = nextTilePos.movedBy(1, Rotation(d));
            if (!board.isTileAllowingMovement(calculatePosition) || calculatePosition == transform.position) {
                continue;
            }

            if (board.isTileEdge(calculatePosition)) {
                calculatePosition = board.complementaryEdgePosition(calculatePosition);
            }

            distances.push_back(std::make_pair(Position::distanceBetween(target, calculatePosition), Rotation(d)));
        }

        std::sort(distances.begin(), distances.end(), [ ](auto & lhs, auto & rhs) {
            return std::tie(lhs.first, lhs.second) < std::tie(rhs.first, rhs.second);
            });

        nextRotation = distances.front().second;
        return;
    }

    for (size_t d = 0; d < 4; d++) {
        Rotation processingRotation(d);
        Position calculatePosition = nextTilePos.movedBy(1, processingRotation);
        if (board.isTileAllowingMovement(calculatePosition) && calculatePosition != transform.position) {
            nextRotation = processingRotation;
            return;
        }
    }

    nextRotation = currentDirection.opposite();
}

Position Enemy::calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (frightened) {
        size_t randX = arc4random_uniform(board.getSizeX());
        size_t randY = arc4random_uniform(board.getSizeY());
        return Position(randX, randY);
    } else if (scatter) {
        return scatterTarget;
    }
    return Position();
}

Enemy::Enemy(const Transform & initial, const Position & scatterPos, bool a) : Entity(initial, a), frightened(false), scatter(false), currentDirection(initial.rotation), scatterTarget(scatterPos) { }

Enemy::~Enemy() { }

void Enemy::move(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (!alive) {
        return;
    }
    
    transform.rotation = currentDirection;
    transform.moveBy(1);
    currentDirection = nextRotation;

    Position target;

    if (frightened || scatter) {
        target = Enemy::calculateTarget(board, playerTransform, specialPos);
    } else {
        target = calculateTarget(board, playerTransform, specialPos);
    }

    calculateNextDirection(board, target);
}

void Enemy::toggleScatter() {
    scatter = !scatter;
    nextRotation = currentDirection.opposite();
}

void Enemy::toggleFrighten() {
    frightened = !frightened;
    nextRotation = currentDirection.opposite();

}

std::pair<char, NCColors::ColorPairs> Enemy::displayEntity() {
    return std::make_pair('&', NCColors::ColorPairs::basic);
}
