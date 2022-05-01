#include "Entity.h"

Entity::Entity(const Transform & initial, bool a = false) : transform(initial), alive(a) { };

Entity::~Entity() { };

void Entity::reposition(const Transform & trans) {
    transform = trans;
}

const Position & Entity::getPosition() const {
    return transform.position;
}