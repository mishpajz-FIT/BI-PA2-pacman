#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Board.h"

class Player : public Entity {
public:
    Player(const Transform & initial, bool a = true);

    void move(const Board & board);
    void rotate(const Rotation & to);

};

#endif /* PLAYER_H */