#include "View.h"

View::View() : needsRefresh(true) { }
View::~View() { }

void View::getWindowSize(WINDOW * forWindow) {
    int maxY, maxX;
    getmaxyx(forWindow, maxY, maxX);
    if (maxY != ySize || maxX != xSize) {
        sizeChanged = true;
        ySize = maxY;
        xSize = maxX;
    } else {
        sizeChanged = false;
    }
}

void View::setNeedsRefresh() {
    needsRefresh = true;
}