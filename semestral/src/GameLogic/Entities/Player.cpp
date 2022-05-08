#include "Player.h"

bool Player::ifPossibleChangeTransform(const Transform & to, const Board & board) {
    if (board.isTileAllowingMovement(to.position)) {
        reposition(to);
        return true;
    }
    return false;
}

Player::Player(const Transform & initial, bool a) : Entity(initial, a) { }

void Player::move(const Board & board) {
    Transform newTransform = transform;
    newTransform.rotation = nextRotation;
    newTransform.moveBy(1);
    if (!ifPossibleChangeTransform(newTransform.position, board)) {
        newTransform = transform.movedBy(1);
        if (!ifPossibleChangeTransform(newTransform.position, board)) {
            rotate(transform.rotation);
        }
    }
}

void Player::rotate(const Rotation & to) {
    nextRotation = to;
}

char Player::displayChar() {
    switch (transform.rotation.direction) {
        case Rotation::Direction::up:
            return 'v';
        case Rotation::Direction::left:
            return '>';
        case Rotation::Direction::down:
            return '^';
        case Rotation::Direction::right:
            return '<';
        default:
            break;
    }

    return ' ';
}
