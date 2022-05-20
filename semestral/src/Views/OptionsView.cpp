#include "OptionsView.h"

OptionsView::OptionsView() : View() {
    minSizeY = 25;
    minSizeX = 50;
}

OptionsView::~OptionsView() { }

void OptionsView::draw(WINDOW * intoWindow) {
    getWindowSize(intoWindow);

    if (!needsRefresh) {
        return;
    }

    wclear(intoWindow);
    box(intoWindow, 0, 0);

    mvwprintw(intoWindow, 1, 1, titleText.c_str());

    if (warningDisplayed) {
        mvwprintw(intoWindow, sizeY - 3, 1, warningText.c_str());
    }

    wmove(intoWindow, sizeY - 2, 1);

    wrefresh(intoWindow);
    needsRefresh = false;
}

OptionsView * OptionsView::clone() const {
    return new OptionsView(*this);
}
