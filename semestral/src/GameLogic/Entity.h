#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"

class Entity {
protected:
    Transform transform;
    bool alive;

public:
    Entity(const Transform & initial, bool a = false);
    virtual ~Entity();

    virtual void reposition(const Transform & trans);

    virtual const Position & getPosition() const {
        return transform.position;
    }

};

#endif /* ENTITY_H */