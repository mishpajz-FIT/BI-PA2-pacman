#include "PositionRotation.h"
#include <stdexcept>

Position::Position() : x(0), y(0) { }

Position::Position(int newX, int newY) : x(newX), y(newY) { }

Rotation::Rotation() : direction(0) { }

Rotation::Rotation(int dir) {
    if (direction >= 4) {
        throw std::out_of_range("Rotation direction is out of range!");
    }

    direction = dir;
}

