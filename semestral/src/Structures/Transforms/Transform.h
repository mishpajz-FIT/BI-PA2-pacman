#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>

#include "Structures/Transforms/Position.h"
#include "Structures/Transforms/Rotation.h"

struct Transform {
public:
    Position position;
    Rotation rotation;

    Transform(const Position & pos = Position(), const Rotation & rot = Rotation());

    Transform & moveBy(int by);
    Transform movedBy(int by) const;
};


#endif /* TRANSFORM_H */