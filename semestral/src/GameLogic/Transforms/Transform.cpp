#include "Transform.h"

Transform::Transform(const Position & pos, const Rotation & rot) : position(pos), rotation(rot) { }

Transform & Transform::moveBy(int by) {
    position.moveBy(by, rotation);
    return (*this);
}

Transform Transform::movedBy(int by) const {
    Transform newTransform(*this);
    newTransform.moveBy(by);
    return newTransform;
}
