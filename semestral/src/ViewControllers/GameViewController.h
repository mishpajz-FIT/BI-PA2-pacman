#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include "ViewController.h"
#include "Game.h"
#include <memory>
#include <ncurses.h>
#include "LayoutView.h"

class GameViewController : public ViewController {
    std::unique_ptr<Game> game;

    enum GameStatePhase {
        optionsLoading,
        mapLoading,
        playing
    };
    GameStatePhase phase;

    LayoutView layoutView;

public:
    GameViewController();

    void update() override;

    void draw() override;

    void setNeedsRefreshToSubviews();
};

#endif /* GAMEVIEWCONTROLLER_H */
