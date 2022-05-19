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
#include "GameSettings.h"

#define GAME_GHOSTCOUNT 4

class Game {
    friend class GameView;
    friend class GameDetailView;

private:
    GameSettings settings;

    bool needsRedraw;

    Timer timer;
    bool paused;

    std::unique_ptr<Board> board;

    std::unique_ptr<Player> player;
    std::array<std::unique_ptr<Enemy>, GAME_GHOSTCOUNT> ghosts;

    unsigned long score;

    bool bonusOut;

    void detectCollisions();

    void movePlayer();
    void moveEnemy(bool fright = false);

    void toggleScatter();

    unsigned int frightenActivated;
    const double frightenSpeedMultiplier;
    void toggleFrighten(bool on);

    void createBonus();

public:
    Game(const GameSettings & gameSettings, double frightenMultiplier);

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