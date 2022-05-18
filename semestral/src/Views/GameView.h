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

    void drawGameElement(WINDOW * intoWindow, const Position & at, unsigned int colorPair, char c);

    void drawBoard(WINDOW * intoWindow);
    void drawPlayer(WINDOW * intoWindow);
    void drawEnemies(WINDOW * intoWindow);

public:
    GameView(Game * game);
    virtual ~GameView();

    void draw(WINDOW * intoWindow = stdscr) override;

    GameView * clone() const override;
};

#endif /* GAMEVIEW_H */