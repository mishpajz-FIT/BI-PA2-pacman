#ifndef PLAYER_H
#define PLAYER_H

#include "Transform.h"

class Player {
protected:
    Transform transform;
    bool alive;

public:
    Player(const Transform & initial);

    void move(const Board & board);
    void move(const Position & pos);
    void rotate(const Rotation & to);

    const Position & getPosition() const;
}

#endif /* PLAYER_H */