#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Board.h"

class Enemy : public Entity {
protected:
    bool frightened;
public:

    Enemy(const Transform & initial) : Entity(initial) { };
    virtual ~Enemy() { };

    virtual void move(const Board & board, const Transform & playerTransform, const Position & specialPos = Position(), bool scatter = false) = 0;
};

#endif /* ENEMY_H */