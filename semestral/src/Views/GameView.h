#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "View.h"
#include "Game.h"

class GameView : public View {
protected:
    Game * gameToDraw;

public:
    GameView();
    virtual ~GameView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setGameToDraw(Game * game);
};

#endif /* GAMEVIEW_H */