#ifndef GAME_H
#define GAME_H

#include "Timer.h"
#include "Player.h"
#include "Ghosts.h"
#include <array>
#include <memory>
#include <string>
#include <ncurses.h>

class Game {
    friend class GameView;
private:
    bool needsRedraw;

    Timer timer;

    std::unique_ptr<Board> board;

    std::unique_ptr<Player> player;
    std::array<std::unique_ptr<Enemy>, 4> ghosts;


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

    void begin() {
        timer.togglePause();
    }

    void update();

    unsigned int getDimensionX();
    unsigned int getDimensionY();
};

#endif /* GAME_H */