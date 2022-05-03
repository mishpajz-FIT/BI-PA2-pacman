#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Position.h"
#include "Rotation.h"
#include <cmath>

struct Transform {
public:
    Position position;
    Rotation rotation;

    Transform(const Position & pos = Position(), const Rotation & rot = Rotation());

    Transform & moveBy(int by);
    Transform movedBy(int by);
};


#endif /* TRANSFORM_H */