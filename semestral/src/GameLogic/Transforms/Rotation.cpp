#include "Rotation.h"

Rotation::Rotation(Rotation::Direction dir) : direction(dir) { }

Rotation::Rotation() : direction(0) { }

Rotation::Rotation(size_t d) : direction(0) {
    if (d > 3) {
        throw std::invalid_argument("rotation: invalid direction");
    }

    direction = static_cast<Direction>(d);
}

Rotation Rotation::opposite() {
    size_t newDirValue = 0;
    if (direction < 2) {
        newDirValue = 4 + direction - 2;
    } else {
        newDirValue = direction - 2;
    }
    return Rotation(newDirValue);
}