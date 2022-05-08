#include "Enemy.h"

void Enemy::calculateNextDirection(const Board & board, const Position & target) {
    Position nextTilePos = transform.position.movedBy(1, currentDirection);
    if (board.isTileCrossroad(nextTilePos)) {
        std::vector<std::pair<double, Rotation>> distances;

        for (size_t d = 0; d < 4; d++) {
            Position calculatePosition = nextTilePos.movedBy(1, Rotation(d));
            Position calculatePositionAfterTeleport = board.complementaryEdgePosition(calculatePosition);
            if (calculatePositionAfterTeleport != transform.position && board.isTileAllowingMovement(calculatePositionAfterTeleport)) {
                distances.push_back(std::make_pair(Position::distanceBetween(target, calculatePositionAfterTeleport), Rotation(d)));
            }
        }

        std::sort(distances.begin(), distances.end(), [ ](auto & lhs, auto & rhs) {
            return std::tie(lhs.first, lhs.second) < std::tie(rhs.first, rhs.second);
            });

        nextRotation = distances.front().second;
        return;
    }

    for (size_t d = 0; d < 4; d++) {
        Position calculatePosition = nextTilePos.movedBy(1, Rotation(d));
        if (calculatePosition != transform.position && board.isTileAllowingMovement(calculatePosition)) {
            nextRotation = Rotation(d);
            return;
        }
    }

    nextRotation = transform.rotation.opposite();
}

Position Enemy::calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (frightened) {
        std::srand(std::time(nullptr));
        size_t randX = rand() % board.getSizeX();
        size_t randY = rand() % board.getSizeY();
        return Position(randX, randY);
    } else if (scatter) {
        return scatterTarget;
    }
    return Position();
}

Enemy::Enemy(const Transform & initial, const Position & scatterPos, bool a) : Entity(initial, a), scatterTarget(scatterPos) { }

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
    if (!frightened) {
        currentDirection = transform.rotation.opposite();
    }
}

void Enemy::toggleFrighten() {
    frightened = !frightened;
    if (!scatter) {
        currentDirection = transform.rotation.opposite();
    }
}

char Enemy::displayChar() {
    return '&';
}
