#ifndef GAMEDETAILVIEW_H
#define GAMEDETAILVIEW_H

#include "View.h"
#include "Game.h"


class GameDetailView : public View {
protected:
    Game * gameToDraw;

public:
    GameDetailView(Game * game);
    virtual ~GameDetailView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setWarning(bool to, std::string text = "") override;

    GameDetailView * clone() const override;
};

#endif /* GAMEDETAILVIEW_H */