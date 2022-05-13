#include "View.h"

View::View(std::function<void()> refreshCallback) : needsRefresh(true), ableToDisplay(true), sizeY(0), sizeX(0), sizeChanged(true), needsRefreshCallback(refreshCallback) { }
View::~View() { }

void View::getWindowSize(WINDOW * forWindow) {
    unsigned int maxY, maxX;
    getmaxyx(forWindow, maxY, maxX);
    sizeChanged = false;
    if ((maxY != sizeY) || (maxX != sizeX)) {
        sizeChanged = true;
        sizeY = maxY;
        sizeX = maxX;
    }
}

void View::setNeedsRefresh() {
    needsRefresh = true;
}

bool View::isAbleToDisplay() {
    return ableToDisplay;
}
