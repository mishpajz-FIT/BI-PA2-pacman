#include "Views/SecondaryViews/GameDetailView.h"
#include "Utilities/NCColors.h"

void GameDetailView::drawHint(WINDOW * intoWindow) {
    wattron(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
    mvwprintw(intoWindow, sizeY - 7, 1, "w, s, a, d - movement");
    mvwprintw(intoWindow, sizeY - 6, 1, "p - pause");
    mvwprintw(intoWindow, sizeY - 5, 1, "(p) q - quit");
    wattroff(intoWindow, COLOR_PAIR(NCColors::ColorPairs::hint));
}

GameDetailView::GameDetailView(Game * game) : SecondaryView(), gameToDraw(game) {
    titleText = "PACMAN";

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

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    wattron(intoWindow, A_BOLD);
    mvwprintw(intoWindow, 1, centeredXForText(titleText), titleText.c_str());
    wattroff(intoWindow, A_BOLD);

    if (isAbleToDisplay()) {
        std::string displayString = "score: ";
        displayString += std::to_string(gameToDraw->getScore());
        mvwprintw(intoWindow, 3, 1, displayString.c_str());

        displayString = "lives: ";
        displayString += std::to_string(gameToDraw->getLives());
        displayString += "x <3";
        mvwprintw(intoWindow, 4, 1, displayString.c_str());
    }

    drawHint(intoWindow);

    if (warningDisplayed) {
        wattron(intoWindow, A_DIM);
        mvwprintw(intoWindow, sizeY - 2, 1, warningText.c_str());
        wattroff(intoWindow, A_DIM);
    }

    wnoutrefresh(intoWindow);
    needsRefresh = false;
}

GameDetailView * GameDetailView::clone() const {
    return new GameDetailView(*this);
}