#include "GameLogic/Entities/Enemy.h"

void Enemy::calculateNextDirection(const Board & board, const Position & target) {
    Position nextTilePos = transform.position.movedBy(1, currentDirection);

    if (board.isTileEdge(nextTilePos)) {
        nextTilePos = board.complementaryEdgePosition(nextTilePos);
    }

    if (board.isTileCrossroad(nextTilePos)) {
        std::vector<std::pair<double, Rotation>> distances;

        for (size_t d = 0; d < 4; d++) {
            Rotation processingRotation(d);
            Position calculatePosition = nextTilePos.movedBy(1, processingRotation);
            if (!board.isTileAllowingMovement(calculatePosition)
                || (processingRotation == currentDirection.opposite())) {
                continue;
            }

            if (board.isTileEdge(calculatePosition)) {
                calculatePosition = board.complementaryEdgePosition(calculatePosition);
            }

            distances.push_back(std::make_pair(Position::distanceBetween(target, calculatePosition), processingRotation));
        }

        std::sort(distances.begin(), distances.end(), [ ](auto & lhs, auto & rhs) {
            return std::tie(lhs.first, lhs.second) < std::tie(rhs.first, rhs.second);
            });

        // If intelligence is low, enemy will choose non-ideal path in 1/4 times
        if (intelligence == 0) {
            srand(time(0));
            if ((rand() % 4) == 0) {
                nextRotation = distances.back().second;
                return;
            }
        }

        nextRotation = distances.front().second;
        return;
    }

    for (size_t d = 0; d < 4; d++) {
        Rotation processingRotation(d);
        Position calculatePosition = nextTilePos.movedBy(1, processingRotation);

        if (board.isTileAllowingMovement(calculatePosition)
            && (processingRotation != currentDirection.opposite())) {
            nextRotation = processingRotation;
            return;
        }
    }

    nextRotation = currentDirection.opposite();
}

Position Enemy::calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (frightened) {

        // Different frightened targets based on intelligence
        if (intelligence == 0) {
            Position behindPlayer = playerTransform.position;
            behindPlayer.moveBy(4, playerTransform.rotation.opposite());
            return behindPlayer;
        } else if (intelligence == 1) {
            srand(time(0));
            size_t randX = rand() % (board.getSizeX());
            size_t randY = rand() % (board.getSizeY());
            return Position(randX, randY);
        }

        Position frightenTarget;
        if ((unsigned long)(playerTransform.position.x) < board.getSizeX()) {
            frightenTarget.x = board.getSizeX();
        } else {
            frightenTarget.x = 0;
        }

        if ((unsigned long)(playerTransform.position.y) < board.getSizeY()) {
            frightenTarget.y = board.getSizeY();
        } else {
            frightenTarget.y = 0;
        }

        return frightenTarget;

    } else if (scatter) {
        return scatterTarget;
    }
    return Position();
}

Enemy::Enemy(
    const Transform & initial,
    const Position & scatterPos,
    bool a,
    unsigned int intelligenceLevel)
    :
    Entity(initial, a),
    intelligence(intelligenceLevel),
    frightened(false), scatter(false),
    currentDirection(initial.rotation),
    scatterTarget(scatterPos) { }

Enemy::~Enemy() { }

void Enemy::move(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (!alive) {
        return;
    }
    
    transform.rotation = currentDirection;
    transform.moveBy(1);
    currentDirection = nextRotation;

    if (board.isTileEdge(transform.position)) {
        transform.position = board.complementaryEdgePosition(transform.position);
    }


    Position target;
    if (frightened || scatter) {
        target = Enemy::calculateTarget(board, playerTransform, specialPos);
    } else {
        target = calculateTarget(board, playerTransform, specialPos);
    }

    calculateNextDirection(board, target);
}

void Enemy::toggleScatter() {
    if (!frightened) {
        nextRotation = currentDirection.opposite();
    }
    scatter = !scatter;
}

void Enemy::toggleFrighten() {
    if (!frightened) {
        nextRotation = currentDirection.opposite();
    }
    frightened = !frightened;
}

bool Enemy::isFrightened() {
    return frightened;
}

std::pair<char, NCColors::ColorPairs> Enemy::displayEntity() {
    if (frightened) {
        return std::make_pair('&', NCColors::ColorPairs::ghostFrighten);
    }
    return std::make_pair('&', NCColors::ColorPairs::basic);
}
