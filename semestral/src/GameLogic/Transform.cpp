#include "Transform.h"
#include <stdexcept>

Position::Position() : x(0), y(0) { }

Position::Position(int newX, int newY) : x(newX), y(newY) { }

Position & Position::moveByOne(Rotation inRotation) {
    switch (inRotation) {
        case Rotation::up:
            y--;
            break;
        case Rotation::down:
            y++;
            break;
        case Rotation::right:
            x++;
            break;
        case Rotation::left:
            x--;
            break;
    }

    return (*this);
}

double Position::distanceBetween(const Position & lhs, const Position & rhs) {
    double disX = rhs.x - lhs.x;
    double disY = rhs.y - lhs.y;
    return hypot(disX, disY);
}

Transform::Transform(const Position & pos, const Rotation & rot) : position(pos), rotation(rot) { }

Transform & Transform::moveByOne() {
    position.moveByOne(rotation);
    return (*this);
}

Transform Transform::moveByOne(const Transform & trans) {
    Transform newTransform(trans);

    return newTransform.moveByOne();
}
