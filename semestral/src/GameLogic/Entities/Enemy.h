#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "GameLogic/Entities/Entity.h"
#include "GameLogic/Board.h"

class Enemy : public Entity {
protected:
    unsigned int intelligence;

    bool frightened;
    bool scatter;

    Rotation currentDirection;

    Position scatterTarget;

    void calculateNextDirection(const Board & board, const Position & target);

    virtual Position calculateTarget(const Board & board, const Transform & playerTransform, const Position & specialPos = Position());

public:

    Enemy(const Transform & initial, const Position & scatterPos, bool a = false, unsigned int intelligenceLevel = 1);
    virtual ~Enemy();

    void move(const Board & board, const Transform & playerTransform, const Position & specialPos = Position());

    void toggleScatter();
    void toggleFrighten();

    bool isFrightened();

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* ENEMY_H */