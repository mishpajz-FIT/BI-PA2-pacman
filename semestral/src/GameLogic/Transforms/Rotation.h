#ifndef ROTATION_H
#define ROTATION_H

#include <stdexcept>

struct Rotation {
    enum Direction : size_t {
        up = 0,
        left,
        down,
        right
    };

    Direction direction;

    Rotation(Direction dir);
    Rotation();
    Rotation(size_t d);

    Rotation opposite();
};

#endif /* ROTATION_H */
