#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Board.h"
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>


class Enemy : public Entity {
protected:
    bool frightened;
    bool scatter;

    Rotation currentDirection;

    Position scatterTarget;

    void calculateNextDirection(const Board & board, const Position & target);

    virtual Position calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos = Position());

public:

    Enemy(const Transform & initial, const Position & scatterPos, bool a = false);
    virtual ~Enemy();

    void move(const Board & board, const Transform & playerTransform, const Position & specialPos = Position());

    void toggleScatter();
    void toggleFrighten();

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* ENEMY_H */