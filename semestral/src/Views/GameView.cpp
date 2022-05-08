#include "GameView.h"

GameView::GameView() : View(), gameToDraw(nullptr) {
    ableToDisplay = false;
}

GameView::~GameView() { }

void GameView::draw(WINDOW * intoWindow) {
    if (!ableToDisplay) {
        if (!needsRefresh) {
            return;
        }
        wclear(intoWindow);
        box(intoWindow, 0, 0);
        wrefresh(intoWindow);
        needsRefresh = false;
        return;
    }

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);
    //TODO: draw game
    wrefresh(intoWindow);
    needsRefresh = false;
}

void GameView::setGameToDraw(Game * game) {
    if (game == nullptr) {
        ableToDisplay = false;
    } else {
        ableToDisplay = true;
    }

    setNeedsRefresh();
    gameToDraw = game;
}
