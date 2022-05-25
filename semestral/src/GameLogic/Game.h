#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include <optional>

#include "Utilities/Timer.h"
#include "GameLogic/Entities/Player.h"
#include "GameLogic/Entities/Ghosts/Ghosts.h"
#include "Utilities/Contexts/GameSettings.h"

class Game {
    friend class GameView;
    friend class GameDetailView;

private:
    GameSettings settings;

    bool needsRedraw;
    std::vector<Position> diffRedraw;

    Timer timer;

    std::unique_ptr<Board> board;

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> ghosts;

    const unsigned int enemyIntelligence;

    unsigned long score;
    unsigned int lives;

    unsigned int killStreak;
    void detectCollisions();

    void movePlayer();
    void moveEnemy(bool fright = false);

    void toggleScatter();

    unsigned int frightenActivated;
    const double frightenSpeedMultiplier;
    void toggleFrighten(bool on);

    void createBonus();

public:
    Game(
        const GameSettings & gameSettings,
        double frightenMultiplier,
        unsigned int livesAmount,
        unsigned int enemyLevel = 1);

    void loadMap(const Board & map);

    void restart();

    void update(std::optional<Rotation> keyPressDirection);

    unsigned int getDimensionX();
    unsigned int getDimensionY();

    void togglePause();
    bool isPaused();

    unsigned long getScore();
    unsigned int getLives();
    unsigned int getCoinsRemaining();

    bool doesNeedRefresh();
};

#endif /* GAME_H */