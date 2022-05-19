#ifndef GAMEVIEWCONTROLLER_H
#define GAMEVIEWCONTROLLER_H

#include "ViewController.h"
#include "Game.h"
#include <memory>
#include <ncurses.h>
#include "LayoutView.h"
#include <optional>

class GameViewController : public ViewController {
protected:
    std::unique_ptr<Game> game;

    enum GameStatePhase {
        settingsLoading,
        mapLoading,
        playing
    };
    GameStatePhase phase;

    LayoutView layoutView;

    std::optional<Rotation> getPlayerRotationFromKey(int c);

public:
    GameViewController();

    void update() override;

    void draw() override;

    void setNeedsRefreshToSubviews();
};

#endif /* GAMEVIEWCONTROLLER_H */
