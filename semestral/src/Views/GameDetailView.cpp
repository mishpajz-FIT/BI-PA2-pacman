#include "GameDetailView.h"

GameDetailView::GameDetailView() : View() {
    minSizeY = 25;
    minSizeX = 50;
    titleText = "PacMan";
}
GameDetailView::~GameDetailView() { }

void GameDetailView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }


    curs_set(0);
    noecho();

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    mvwprintw(intoWindow, 1, 1, titleText.c_str());

    if (warningDisplayed) {
        mvwprintw(intoWindow, sizeY - 2, 1, warningText.c_str());
    }

    wrefresh(intoWindow);
    needsRefresh = false;
}

void GameDetailView::setWarning(bool to, std::string text) {
    if (warningDisplayed != to || warningText != text) {
        warningDisplayed = to;
        warningText = text;
        setNeedsRefresh();
    }
}

GameDetailView * GameDetailView::clone() const {
    return new GameDetailView(*this);
}