#include "Position.h"

Position::Position() : x(0), y(0) { }

Position::Position(int newX, int newY) : x(newX), y(newY) { }

Position & Position::moveBy(int by, Rotation inRotation) {
    switch (inRotation.direction) {
        case Rotation::Direction::up:
            y -= by;
            break;
        case Rotation::Direction::down:
            y += by;
            break;
        case Rotation::Direction::right:
            x += by;
            break;
        case Rotation::Direction::left:
            x -= by;
            break;
    }

    return (*this);
}

Position Position::movedBy(int by, Rotation inRotation) {
    Position newPosition(*this);
    newPosition.moveBy(by, inRotation);
    return newPosition;
}

double Position::distanceBetween(const Position & lhs, const Position & rhs) {
    double disX = rhs.x - lhs.x;
    double disY = rhs.y - lhs.y;
    return hypot(disX, disY);
}

bool Position::operator == (const Position & rhs) {
    return (x == rhs.x) && (y == rhs.y);
}

bool Position::operator != (const Position & rhs) {
    return !((*this) == rhs);
}