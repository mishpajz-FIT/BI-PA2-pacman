#include "Transform.h"

Transform::Transform(const Position & pos, const Rotation & rot) : position(pos), rotation(rot) { }

Transform & Transform::moveByOne() {
    position.moveByOne(rotation);
    return (*this);
}

Transform Transform::moveByOne(const Transform & trans) {
    Transform newTransform(trans);

    return newTransform.moveByOne();
}
