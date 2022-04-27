#ifndef ENEMY_H
#define ENEMY_H

#include "Transform.h"
#include <ncurses.h>

class Enemy {
protected:
    Transform transform;
    bool alive;
    bool frightened;

    Transform initialTransform;
public:

    Enemy(const Position & pos);

    virtual void move(const Board & board, const Transform & playerTransform, const Position & specialPlace = Position(), bool scatter = false) = 0;

    virtual const Position & getPosition() = 0;
};

#endif /* ENEMY_H */