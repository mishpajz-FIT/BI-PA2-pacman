#include "GameLogic/Entities/Enemy.h"

void Enemy::calculateNextDirection(const Board & board, const Position & target) {
    Position nextTilePos = transform.position.movedBy(1, currentDirection); //< Get tile to
    // which this enemy will move to on next move

    // If next tile is at edge, it is a teleport and position behind teleport should be 
    // used to calculate nextRotation
    if (board.isTileEdge(nextTilePos)) {
        nextTilePos = board.complementaryEdgePosition(nextTilePos);
    }

    // If next tile is crossroad
    if (board.isTileCrossroad(nextTilePos)) {
        std::vector<std::pair<double, Rotation>> distances;

        // Check in every direction except for tile this enemy is currently on and
        // calculate distance from each tile to target
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

            // Calculate distance
            distances.push_back(std::make_pair(Position::distanceBetween(target, calculatePosition), processingRotation));
        }

        // Sort tiles by lowest distance
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

        // Else set direction to tile which has smallest distance to target
        nextRotation = distances.front().second;
        return;
    }

    // If not at crossroad, find the direction in which it is possible to move
    // except for tile this enemy is currently on
    for (size_t d = 0; d < 4; d++) {
        Rotation processingRotation(d);
        Position calculatePosition = nextTilePos.movedBy(1, processingRotation);

        if (board.isTileAllowingMovement(calculatePosition)
            && (processingRotation != currentDirection.opposite())) {
            nextRotation = processingRotation;
            return;
        }
    }

    // If movement is possible in no other direction, turn around
    nextRotation = currentDirection.opposite();
}

Position Enemy::calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos) {
    if (frightened) {

        // Different frightened targets based on intelligence
        if (intelligence == 0) { // If intelligence is low, move behind player
            Position behindPlayer = playerTransform.position;
            behindPlayer.moveBy(4, playerTransform.rotation.opposite());
            return behindPlayer;
        } else if (intelligence == 1) { // If intelligence is medium, move to random position
            srand(time(0));
            size_t randX = rand() % (board.getSizeX());
            size_t randY = rand() % (board.getSizeY());
            return Position(randX, randY);
        }

        // If intelligence is high, move to quadrant of board that is opposite to 
        // the quadrant in which is player

        Position frightenTarget;
        // x coord calculation
        if ((unsigned long)(playerTransform.position.x) < board.getSizeX()) {
            frightenTarget.x = board.getSizeX();
        } else {
            frightenTarget.x = 0;
        }

        // y coord calculation
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
    
    // Enemy's transform reflects rotation of the previous movement.

    // Set transform's rotation to currentDirection and move in this direction,
    // then set currentDirection to nextRotation
    transform.rotation = currentDirection;
    transform.moveBy(1);
    currentDirection = nextRotation;

    //If at edge, teleport to the other side
    if (board.isTileEdge(transform.position)) {
        transform.position = board.complementaryEdgePosition(transform.position);
    }

    // Calculate new nextRotation
    Position target = calculateTarget(board, playerTransform, specialPos);
    calculateNextDirection(board, target);
}

void Enemy::toggleScatter(const Board & board) {
    //If not frightened and not at edge, turn around
    if (!frightened && !board.isTileEdge(transform.position)) {
        nextRotation = currentDirection.opposite();
    }
    scatter = !scatter;
}

void Enemy::toggleFrighten(const Board & board) {
    //If toggling frighten off and not at edge, turn around
    if (!frightened && !board.isTileEdge(transform.position)) {
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
