#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include "ViewController.h"
#include "Game.h"
#include <memory>
#include <ncurses.h>
#include "LayoutView.h"
#include "GameView.h"
#include "OptionsView.h"

class GameViewController : public ViewController {
    std::unique_ptr<Game> game;

    enum GameStatePhase {
        optionsLoading,
        mapLoading,
        playing
    };
    GameStatePhase phase;

    LayoutView layoutView;
    GameView gameView;
    OptionsView optionsView;

public:
    GameViewController();

    void update() override;

    void draw() override;

    void setNeedsRefreshToSubviews();
};

#endif /* GAMEVIEWCONTROLLER_H */
