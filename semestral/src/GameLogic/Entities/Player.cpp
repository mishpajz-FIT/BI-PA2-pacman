#include "Player.h"

Player::Player(const Transform & initial, bool a) : Entity(initial, a) { }

void Player::move(const Board & board) {
    Transform newTransform = transform;
    newTransform.rotation = nextRotation;
    newTransform = Transform::moveByOne(newTransform);
    if (board.isTileAllowingMovement(newTransform.position)) {
        transform = newTransform;
    } else {
        transform = Transform::moveByOne(transform);
        nextRotation = transform.rotation;
    }
}

void Player::rotate(const Rotation & to) {
    nextRotation = to;
}