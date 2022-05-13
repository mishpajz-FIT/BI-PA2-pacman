#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"
#include "NCColors.h"
#include <tuple>

class Entity {
protected:
    Transform transform;
    Rotation nextRotation;
    bool alive;

public:
    Entity(const Transform & initial, bool a = false);
    virtual ~Entity();

    void reposition(const Transform & trans);

    Transform getTransform() const;

    void toggleAlive();

    bool isAlive();

    virtual std::pair<char, NCColors::ColorPairs> displayEntity() = 0;
};

#endif /* ENTITY_H */