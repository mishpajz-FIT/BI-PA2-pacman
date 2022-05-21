#ifndef GAMEDETAILVIEW_H
#define GAMEDETAILVIEW_H

#include "SecondaryView.h"
#include "Game.h"


class GameDetailView : public SecondaryView {
protected:
    Game * gameToDraw;

    void drawHint(WINDOW * intoWindow) override;

public:
    GameDetailView(Game * game);
    virtual ~GameDetailView();

    void draw(WINDOW * intoWindow = stdscr) override;

    GameDetailView * clone() const override;
};

#endif /* GAMEDETAILVIEW_H */