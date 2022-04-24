#ifndef ENEMY_H
#define ENEMY_H

#include <Coords.h>
#include <ncurses.h>

class Enemy {
private:
    Coords position;
    bool alive;
    bool frightened;

public:

    Enemy(const Coords & pos);

    void display(WINDOW * window) = 0;

    void move(const Maze & maze, const Coords & player, const Coords & place = Coords(), bool scatter = false) = 0;

    Coords getPosition() = 0;
}

#endif /* ENEMY_H */