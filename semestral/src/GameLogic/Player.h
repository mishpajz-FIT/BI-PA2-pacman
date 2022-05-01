#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Board.h"

class Player : public Entity {
private:
    Rotation nextRotation;

public:
    Player(const Transform & initial);

    void move(const Board & board);
    void rotate(const Rotation & to);

};

#endif /* PLAYER_H */