#include "GameDetailView.h"

GameDetailView::GameDetailView(Game * game) : View(), gameToDraw(game) {
    minSizeY = 25;
    minSizeX = 50;
    titleText = "PacMan";

    if (gameToDraw == nullptr) {
        ableToDisplay = false;
    }
}
GameDetailView::~GameDetailView() { }

void GameDetailView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!(needsRefresh || (isAbleToDisplay() && gameToDraw->doesNeedRefresh()))) {
        return;
    }


    curs_set(0);
    noecho();

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    mvwprintw(intoWindow, 1, 1, titleText.c_str());

    if (isAbleToDisplay()) {
        std::string scoreString = "score: ";
        scoreString += std::to_string(gameToDraw->getScore());

        mvwprintw(intoWindow, 3, 1, scoreString.c_str());
    }

    if (warningDisplayed) {
        mvwprintw(intoWindow, sizeY - 2, 1, warningText.c_str());
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

GameDetailView * GameDetailView::clone() const {
    return new GameDetailView(*this);
}