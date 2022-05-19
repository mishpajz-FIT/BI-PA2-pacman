#include "Rotation.h"

Rotation::Rotation(Rotation::Direction dir) : direction(dir) { }

Rotation::Rotation() : direction(up) { }

Rotation::Rotation(size_t d) : direction(up) {
    if (d > 3) {
        throw std::invalid_argument("rotation: invalid direction");
    }

    direction = static_cast<Direction>(d);
}

Rotation Rotation::opposite() const {
    size_t newDirValue = 0;
    if (direction < 2) {
        newDirValue = 4 + direction - 2;
    } else {
        newDirValue = direction - 2;
    }
    return Rotation(newDirValue);
}

bool Rotation::operator < (const Rotation & rhs) const {
    return direction < rhs.direction;
}

bool Rotation::operator == (const Rotation & rhs) const {
    return direction == rhs.direction;
}

bool Rotation::operator != (const Rotation & rhs) const {
    return !((*this) == rhs);
}