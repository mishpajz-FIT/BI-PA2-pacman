#ifndef POSITION_H
#define POSITION_H

#include "Rotation.h"
#include <cmath>

struct Position {
public:
    int x;
    int y;

    Position();

    Position(int newX, int newY);

    Position & moveByOne(Rotation inRotation);
    Position movedByOne(Rotation inRotation);

    static double distanceBetween(const Position & lhs, const Position & rhs);

    bool operator == (const Position & rhs);
    bool operator != (const Position & rhs);
};

#endif /* POSITION_H */
