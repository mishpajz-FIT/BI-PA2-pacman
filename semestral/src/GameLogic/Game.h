#ifndef GAME_H
#define GAME_H

#include "Timer.h"
#include "Player.h"
#include "Ghosts.h"
#include <array>
#include <memory>
#include <string>
#include <ncurses.h>
#include <optional>

#define GAME_GHOSTCOUNT 4

class Game {
    friend class GameView;
    friend class GameDetailView;

private:
    bool needsRedraw;

    Timer timer;
    bool paused;

    std::unique_ptr<Board> board;

    std::unique_ptr<Player> player;
    std::array<std::unique_ptr<Enemy>, GAME_GHOSTCOUNT> ghosts;

    unsigned long score;

    bool bonusOut;

    const unsigned int playerSpeed;
    const unsigned int enemySpeed;

    const unsigned int scatterDuration;
    const unsigned int chaseDuration;
    const unsigned int frightenDuration;

    const unsigned int killDuration;
    const unsigned int bonusPeriod;
    const unsigned int ghostComeOutPeriod;

    void detectCollisions();

    void movePlayer();
    void moveEnemy();

    void toggleScatter();

    void createBonus();

public:
    Game(
        unsigned int playerSpd,
        unsigned int enemySpd,
        unsigned int scatterDur,
        unsigned int chaseDur,
        unsigned int frightenDur,
        unsigned int killDur,
        unsigned int bonusPer,
        unsigned int ghostComeOutPer
    );

    void loadMap(const std::string & filepath);

    void restart();

    void update(std::optional<Rotation> keyPressDirection);

    unsigned int getDimensionX();
    unsigned int getDimensionY();

    void begin();
    void togglePause();
    bool isPaused();

    unsigned long getScore();

    bool doesNeedRefresh();
};

#endif /* GAME_H */