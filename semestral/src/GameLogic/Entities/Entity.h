#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"

class Entity {
protected:
    Transform transform;
    Rotation nextRotation;
    bool alive;

public:
    Entity(const Transform & initial, bool a = false);
    virtual ~Entity();

    void reposition(const Transform & trans);

    Position getPosition() const;

};

#endif /* ENTITY_H */