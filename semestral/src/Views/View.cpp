#include "View.h"

View::View(std::function<void()> refreshCallback) : needsRefresh(true), ableToDisplay(true), sizeY(-1), sizeX(-1), sizeChanged(true), needsRefreshCallback(refreshCallback) { }
View::~View() { }

void View::getWindowSize(WINDOW * forWindow) {
    int maxY, maxX;
    getmaxyx(forWindow, maxY, maxX);
    if (maxY != sizeY || maxX != sizeX) {
        sizeChanged = true;
        sizeY = maxY;
        sizeX = maxX;
    } else {
        sizeChanged = false;
    }
}

void View::setNeedsRefresh() {
    needsRefresh = true;
}

bool View::isAbleToDisplay() {
    return ableToDisplay;
}
