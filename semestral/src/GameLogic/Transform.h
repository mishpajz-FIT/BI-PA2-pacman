#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

enum Rotation {
    up = 0,
    down = 1,
    right = 2,
    left = 3
};

struct Position {
public:
    int x;
    int y;

    Position();

    Position(int newX, int newY);

    Position & moveByOne(Rotation inRotation);

    static double distanceBetween(const Position & lhs, const Position & rhs);
};

struct Transform {
public:
    Position position;
    Rotation rotation;

    Transform(const Position & pos = Position(), const Rotation & rot = Rotation::up);

    Transform & moveByOne();

    static Transform moveByOne(const Transform & trans);
};


#endif /* TRANSFORM_H */