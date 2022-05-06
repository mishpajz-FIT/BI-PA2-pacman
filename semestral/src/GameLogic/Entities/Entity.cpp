#include "Entity.h"

Entity::Entity(const Transform & initial, bool a) : transform(initial), alive(a), nextRotation(initial.rotation) { };

Entity::~Entity() { };

void Entity::reposition(const Transform & trans) {
    transform = trans;
}

Transform Entity::getTransform() const {
    return transform;
}

void Entity::toggleAlive() {
    alive = !alive;
}