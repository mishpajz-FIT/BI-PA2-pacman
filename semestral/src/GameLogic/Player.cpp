#include "Player.h"

Player::Player(const Transform & initial) : Entity(initial), nextRotation(transform.rotation) { }

void Player::move(const Board & board) {
    transform.rotation = nextRotation;
    Transform newTransform = Transform::moveByOne(transform);
    try {
        if (board.tileAt(transform.position) == Board::Tile::Type::space) {
            transform = newTransform;
        }
    }
    catch (BoardException & e) { }
}

void Player::rotate(const Rotation & to) {
    nextRotation = to;
}