#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include "ViewController.h"
#include "Game.h"
#include <memory>
#include <ncurses.h>

class GameViewController : public ViewController {
    std::unique_ptr<Game> game;

    enum GameStatePhase {
        none,
        optionsLoading,
        mapLoading,
        playing
    };
    GameStatePhase phase;
    GameStatePhase prevPhase;

    WINDOW * gameWindow;
    WINDOW * optionWindow;

    void createWindows() override;

    void drawGame();

    void drawOptions();

public:
    GameViewController();

    void update() override;

    void draw() override;

};

#endif /* GAMEVIEWCONTROLLER_H */
