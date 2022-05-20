#include "View.h"

View::View() :
    needsRefresh(true),
    ableToDisplay(true),
    sizeY(0),
    sizeX(0),
    sizeChanged(true),
    minSizeX(0),
    minSizeY(0),
    warningDisplayed(false),
    warningText(""),
    titleText("") { }
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

unsigned int View::centeredXInWindow(unsigned int x) const {
    return ((sizeX - minSizeX) / 2) + x;
}

unsigned int View::centeredYInWindow(unsigned int y) const {
    return ((sizeY - minSizeY) / 2) + y;
}

void View::setNeedsRefresh() {
    needsRefresh = true;
}

bool View::isAbleToDisplay() {
    return ableToDisplay;
}

void View::setWarning(bool to, std::string text) {
    if (warningDisplayed != to || warningText != text) {
        warningDisplayed = to;
        warningText = text;
        setNeedsRefresh();
    }
}

void View::setTitle(std::string text) {
    if (titleText != text) {
        titleText = text;
        setNeedsRefresh();
    }
}

unsigned int View::getMinSizeX() {
    return minSizeX;
}

unsigned int View::getMinSizeY() {
    return minSizeY;
}

ViewException::ViewException(const std::string & message) : runtime_error(message) { }