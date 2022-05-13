#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "View.h"
#include "Game.h"
#include "Board.h"
#include "NCColors.h"
#include <tuple>

class GameView : public View {
protected:
    Game * gameToDraw;

    typedef std::pair<char, NCColors::ColorPairs> DisplayInformation;

public:
    GameView();
    virtual ~GameView();

    void draw(WINDOW * intoWindow = stdscr) override;

    void setGameToDraw(Game * game);
};

#endif /* GAMEVIEW_H */