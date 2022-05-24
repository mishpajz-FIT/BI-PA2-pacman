#ifndef PLAYER_H
#define PLAYER_H

#include "GameLogic/Entities/Entity.h"
#include "GameLogic/Board.h"

class Player : public Entity {
private:
    bool ifPossibleChangeTransform(const Transform & to, const Board & board);

public:
    Player(const Transform & initial, bool a = true);

    void move(const Board & board);
    void rotate(const Rotation & to);

    std::pair<char, NCColors::ColorPairs> displayEntity() override;
};

#endif /* PLAYER_H */