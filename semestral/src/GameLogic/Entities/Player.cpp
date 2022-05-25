#include "GameLogic/Entities/Player.h"

bool Player::ifPossibleChangeTransform(const Transform & to, const Board & board) {
    if (board.isTileAllowingMovement(to.position)) {
        reposition(to);
        return true;
    }
    return false;
}

Player::Player(const Transform & initial, bool a) : Entity(initial, a) { }

void Player::move(const Board & board) {
    // Try to move in direction of next rotation
    Transform newTransform = transform;
    newTransform.rotation = nextRotation;
    newTransform.moveBy(1);

    // If not possible, try to move in direction of previous movement
    if (!ifPossibleChangeTransform(newTransform, board)) {

        newTransform = transform.movedBy(1);
        if (!ifPossibleChangeTransform(newTransform, board)) {
            
            rotate(transform.rotation);
        }
    }

    // If at edge, teleport to other side of board
    if (board.isTileEdge(transform.position)) {
        transform.position = board.complementaryEdgePosition(transform.position);
    }
}

void Player::rotate(const Rotation & to) {
    nextRotation = to;
}

std::pair<char, NCColors::ColorPairs> Player::displayEntity() {
    char c = '0';

    // Display different chars based on direction of movement
    switch (transform.rotation.direction) {
        case Rotation::Direction::up:
            c = 'v';
            break;
        case Rotation::Direction::left:
            c = '>';
            break;
        case Rotation::Direction::down:
            c = '^';
            break;
        case Rotation::Direction::right:
            c = '<';
            break;
    }

    return std::make_pair(c, NCColors::ColorPairs::pacman);
}
