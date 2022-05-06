#include "Entity.h"

Entity::Entity(const Transform & initial, bool a) : transform(initial), nextRotation(initial.rotation), alive(a) { };

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